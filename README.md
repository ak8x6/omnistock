# 📦 OmniStock

<p align="center">
  <img src="https://img.shields.io/badge/Language-C99-A8B9CC?style=flat-square&logo=c&logoColor=white" alt="C99" />
  <img src="https://img.shields.io/badge/Build-GNU%20Make-427819?style=flat-square&logo=gnu&logoColor=white" alt="GNU Make" />
  <img src="https://img.shields.io/badge/Dependencies-None-2ea44f?style=flat-square" alt="No dependencies" />
  <img src="https://img.shields.io/badge/Warnings-Wall%20%2B%20Wextra-orange?style=flat-square" alt="-Wall -Wextra" />
  <img src="https://img.shields.io/badge/License-MIT-blue?style=flat-square" alt="MIT License" />
</p>

> A dependency-free inventory engine in C99 — a permanently-sorted BST catalog, a FIFO order pipeline, and O(1) undo, each behind its own header interface.

---

## 📖 Overview

A warehouse back office runs three concerns at once: a **product catalog** that has to stay ordered for reporting, a **stream of stock movements** that has to be applied in the order it arrived, and an **operator** who occasionally needs to take the last movement back.

OmniStock implements that loop as a single C99 process with no dependency beyond the standard library. The defining decision is that each concern gets the data structure that actually fits it, rather than one container with helpers bolted on:

- The catalog is a **binary search tree keyed on product ID**, so it is permanently sorted — listing it is a traversal, not a sort.
- Stock movements land in a **singly-linked FIFO queue**, so processing order is arrival order and stays auditable.
- Every applied movement is pushed onto a **LIFO stack**, so undo is a pop plus an inverse delta rather than a diff against a snapshot.

Each structure is an isolated compilation unit behind its own header. `main.c` owns the menu loop and wires the three together; it holds no data structure logic of its own.

**Current scope:** state lives in process memory for the life of the session — there is no file or database persistence yet, and the interface is an interactive terminal menu. See [Known Limitations & Roadmap](#-known-limitations--roadmap).

---

## ✨ Capabilities

| Capability | Implementation |
|---|---|
| **Product catalog** | `Product { int id; char name[50]; double price; int qty; }` inserted into a BST keyed on `id` |
| **List by ID** | In-order traversal via `display_inorder()` — O(n), no sort pass required |
| **Point lookup** | Recursive `search()` down the ID tree — O(log n) average |
| **Stock movements** | `Order { id, prodId, change }` enqueued to a linked FIFO; positive = restock, negative = sale |
| **Apply next movement** | `dequeue()` → `search()` → `qty += change`, then push the order onto the undo stack |
| **Repeatable undo** | `pop()` the last applied movement and subtract its delta — unwinds the entire session, not just one step |
| **List by price** | Flatten the BST into a heap array, Quicksort on `price` with Lomuto partitioning, then free the array |
| **Deterministic teardown** | `destroy_tree()`, `destroy_queue()`, and `destroy_stack()` all run before `main` returns; the price view frees its own scratch array |

---

## 🏗️ Architecture

```
┌──────────────────────────────────────────────────────────────────────┐
│                           OmniStock Engine                           │
│                                                                      │
│   ┌──────────────────┐   ┌──────────────────┐   ┌──────────────────┐ │
│   │  InventoryTree   │   │   OrderQueue     │   │   UndoStack      │ │
│   │  (BST by ID)     │   │  (Linked FIFO)   │   │  (LIFO Stack)    │ │
│   │                  │   │                  │   │                  │ │
│   │  O(log n) lookup │   │  FIFO stock      │   │  O(1) push/pop   │ │
│   │  (avg case)      │   │  deltas in order │   │  undo history    │ │
│   └────────┬─────────┘   └────────┬─────────┘   └────────┬─────────┘ │
│            │                      │                      │           │
│            │                      │  dequeue()           │  pop()    │
│            │                      └──────────┬───────────┘           │
│            │                                 ▼                       │
│            │                     node->prod.qty += change            │
│            │                                                         │
│            ▼                                                         │
│   Quicksort (price view): O(n log n) avg                             │
│   in-order flatten → Lomuto partition → price order                  │
└──────────────────────────────────────────────────────────────────────┘
```

The price view reads from the **tree**, not the queue: a price-ordered listing cannot come from traversal because the tree is keyed on ID, so nodes are copied out into a flat array and sorted there. The tree itself is never mutated by the price view.

### Design decisions

**Why a BST for the catalog instead of an array?**
Products are keyed by a numeric ID, and the most frequent read is "list everything in ID order." A BST gives ordered insertion for free — in-order traversal *is* the sorted listing, so there is no sort pass on the reporting path. Lookup during order processing is O(log n) average instead of a linear scan.

**Why a linked queue instead of a ring buffer?**
Movement volume has no natural ceiling for a warehouse day, and picking a capacity at compile time would mean either wasting memory or dropping orders. A singly-linked queue with `front`/`rear` pointers grows on demand and gives O(1) enqueue and O(1) dequeue with no resize copy and no capacity to tune.

**Why a stack for undo instead of storing snapshots?**
A stock movement is already its own inverse: `+12` is undone by `-12`. Storing the delta rather than a copy of the catalog makes undo O(1) in space per operation instead of O(n). LIFO is the correct order because the newest movement has to come off first for the arithmetic to stay coherent.

**Why Quicksort instead of sorting the tree in place?**
The tree is keyed on ID, so price order cannot be produced by any traversal of it. Flattening into a contiguous array makes the comparison loop cache-friendly, and Lomuto partitioning sorts it in place — O(n log n) average with only the recursion stack as overhead, and the array is freed as soon as the view is printed.

---

## 🛠️ Tech Stack

| Layer | Choice | Notes |
|---|---|---|
| Language | C99 (`-std=c99`) | Strict standard mode, no GNU extensions |
| Dependencies | None | `stdio.h` and `stdlib.h` only |
| Build | GNU Make | Pattern rule `bin/%.o: src/%.c` for incremental rebuilds |
| Warnings | `-Wall -Wextra` | Surfaces unused parameters, sign mismatches, and implicit conversions at compile time |
| Include path | `-Iinclude` | Headers resolve by name — no `../` relative paths anywhere in `src/` |
| Interface | Interactive terminal menu | Single process, no command-line arguments |

---

## 🗂️ Project Structure

```
omnistock/
├── include/                 # Public interfaces
│   ├── core_types.h         # Product and Order structs
│   ├── inventory_bst.h      # BSTNode, InventoryTree, insert/search/traverse/destroy
│   ├── order_queue.h        # QueueNode, OrderQueue, enqueue/dequeue/destroy
│   ├── undo_stack.h         # StackNode, UndoStack, push/pop/destroy
│   └── inventory_sort.h     # BST flattening + Quicksort by price
├── src/
│   ├── main.c               # Menu loop; wires the three structures together
│   ├── inventory_bst.c      # BST implementation
│   ├── order_queue.c        # FIFO queue implementation
│   ├── undo_stack.c         # LIFO stack implementation
│   └── inventory_sort.c     # countNodes / toArray / quickSort / display_sorted
├── bin/                     # Build output — created by `make`, not tracked
├── Makefile
├── LICENSE
└── README.md
```

---

## 🚀 Getting Started

**Prerequisites:** GCC and GNU Make.

```bash
git clone https://github.com/ak8x6/omnistock.git
cd omnistock

make              # compiles all five translation units into bin/
./bin/OmniStock.exe

make clean        # remove build artifacts
```

> **Note on the binary name:** the Makefile's `EXEC` target is `bin/OmniStock.exe` on every platform. On Linux and macOS the file runs fine with the extension, or change `EXEC` in the Makefile to drop it.

### A worked session

Adding `204` before `101` and then listing shows the BST doing its job — the listing comes back in ID order without a sort pass:

```
=== OmniStock Menu ===
1. Add Product
2. Display (ID)
3. Add Order
4. Process Order
5. Undo
6. Display (Price)
7. Exit
Choice: 1
ID: 204
Name: Keyboard
Price: 79.50
Quantity: 40
Done!

Choice: 1
ID: 101
Name: Monitor
Price: 189.00
Quantity: 12
Done!

Choice: 2

--- Inventory ---
ID: 101 | Name: Monitor | Price: $189.00 | Stock: 12
ID: 204 | Name: Keyboard | Price: $79.50 | Stock: 40

Choice: 3
Product ID: 101
Change (-/+) : -5
Queued 1

Choice: 4
Processed 1. Stock: 7

Choice: 5
Undid 1. Stock: 12

Choice: 6

--- Sorted by Price ---
ID: 204 | Name: Keyboard | Price: $79.50 | Stock: 40
ID: 101 | Name: Monitor | Price: $189.00 | Stock: 12

Choice: 7
Bye.
```

---

## 📋 CLI Reference

| Option | Action | Under the hood |
|---|---|---|
| `1` | Add Product | `insert()` — BST insertion keyed on ID |
| `2` | Display (ID) | `display_inorder()` — in-order traversal |
| `3` | Add Order | `enqueue()` — restock is positive, sale is negative |
| `4` | Process Order | `dequeue()` → `search()` → apply delta → `push()` |
| `5` | Undo | `pop()` → `search()` → apply inverse delta |
| `6` | Display (Price) | `display_sorted()` — flatten to array, then Quicksort |
| `7` | Exit | Free the tree, queue, and stack, then return |

---

## 🚧 Known Limitations & Roadmap

These are real findings from reading the source, listed roughly by severity. Nothing here is hidden behind a "future work" label — each one is a concrete open item.

- [ ] **`scanf("%s", p.name)` is an unbounded read.** A product name longer than 49 characters overflows the fixed `char name[50]` field. Needs a width specifier (`%49s`) or a `fgets` + trim path. Highest-priority fix in the codebase.
- [ ] **Duplicate product IDs are silently dropped.** `insertRec()` returns the existing node untouched when `p.id` already exists, but `main` prints `Done!` either way — so the operator is told the add succeeded when nothing was stored. Should report a conflict or update the existing record in place.
- [ ] **Stock can go negative.** `node->prod.qty += o.change` is applied with no floor check, so a sale larger than the quantity on hand produces a negative stock level instead of being rejected.
- [ ] **Input validation is inconsistent.** Only the menu-choice `scanf` checks its return value and flushes the input buffer. The ID, price, and quantity reads do not, so a non-numeric entry leaves the field uninitialised and the offending token sitting in the buffer.
- [ ] **No persistence.** The catalog, the pending queue, and the undo history exist only in process memory; exiting discards all of it. A file-backed or SQLite-backed store is the next significant feature.
- [ ] **The BST is never rebalanced.** The `O(log n)` figures are average case. Products entered in ascending ID order — the natural data-entry pattern — degrade the tree into a linked list and lookup into O(n). AVL or red-black rotations would bound it.
- [ ] **Quicksort pivots on the last element.** An inventory already ordered by price hits the O(n²) worst case. Median-of-three or a randomised pivot would fix it.
- [ ] **Internal helpers are not `static`.** `insertRec`, `swapProd`, and `partition` have external linkage with no header declaration, so they leak into the global namespace across translation units.
- [ ] **No point-lookup command.** `search()` is implemented and exercised internally by order processing and undo, but the menu exposes no "find product by ID" option.
- [ ] **No delete or edit path.** Products can be added and their quantity adjusted, but never removed or renamed.
- [ ] **No automated tests.** There is no test target or test source — behaviour has only been verified interactively.
- [ ] **`string.h` is included but unused** in both `main.c` and `inventory_bst.c`.

---

## 👤 Author

**Ahmad Kassem**
GitHub: [@ak8x6](https://github.com/ak8x6)

---

## 📄 License

Released under the MIT License — see [LICENSE](LICENSE) for details.
