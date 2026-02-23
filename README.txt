
The core of this mini engine is the **slotted page**, implemented in `Page.h`/`Page.cpp`. 
A slotted page is a fixed-size block of memory (here, 4KB) that stores multiple records. 
Each record is placed at the “end” of the free space, and the page maintains a **slot directory 
at the front that tracks where each record is stored and its length. This allows the page to efficiently insert, 
read, and later delete or update records without moving all other data. 

In the minimal implementation, `insert()` checks if there is enough free space, copies the record into the page, 
updates the slot directory, and increases the free space pointer. The `read()` function uses the slot directory to 
locate a record by its slot ID and return the data. This layout mirrors what production databases like PostgreSQL use for row storage.

On top of the slotted page, the block pool (`BlockPool.h`/`BlockPool.cpp`) manages multiple pages in memory. 
It is essentially an in-memory array of pages that simulates a small buffer manager. The block pool allows you 
to fetch a page by its ID and provides a place to store multiple pages, giving a basic simulation of multiple 
blocks in a database table. 

Although there is no eviction or disk persistence yet, this design separates page storage from the higher-level table logic, 
which is important for future extensions like LRU replacement, flushing to disk, or maintaining a larger buffer pool for 
analytics workloads.

The `main.cpp` file demonstrates how to interact with the engine. The first example inserts several small records into the 
first page and then reads them back to verify correctness. This tests that the slotted page can store multiple variable-length 
records and that the slot directory correctly maps slot IDs to the records. 

The second example repeatedly inserts new records until the page is full, testing that the engine correctly handles the 
boundary condition when there is no more free space. Together, these examples validate the fundamental mechanics of the 
page layout, slot tracking, and free space calculation in a simple, observable way.

Overall, this minimal design is intentionally layered and modular. 
The slotted page handles individual page storage, the block pool handles page management in memory, 
and the main function demonstrates usage. This separation makes it easy to incrementally add more advanced features: 
you could add disk persistence via a `DiskManager`, 
introduce an LRU buffer pool, implement MVCC and transaction support, 
or even switch to a hybrid columnar layout for analytic workloads compatible with engines like Trino. 
The minimal code is small enough to understand but structured so that each component can evolve into a production-style database engine.
