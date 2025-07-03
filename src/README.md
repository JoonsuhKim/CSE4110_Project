# Convenience Store DB Project

This project implements a supply-chain database system for a chain of convenience stores. It includes:

- Conceptual design (ERD)
- Relational schema in BCNF
- Physical schema in MySQL
- C application for query execution

## Setup Instructions

### 1. Create Database

Using MySQL Workbench or CLI:
```sql
SOURCE schema.sql;
SOURCE sample_data.sql;
```

### 2. Compile and Run

```bash
gcc main.c -o main \
  -I[mysql_include_path] \
  -L[mysql_library_path] \
  -lmysqlclient

DYLD_LIBRARY_PATH=[mysql_library_path] ./main
```

> Replace `[mysql_include_path]` and `[mysql_library_path]` with your actual MySQL installation paths.

## Sample Queries

- Stores carrying a specific product
- Top-selling items by store
- Vendor statistics
- Inventory alerts
- Customer purchase patterns

## Development Tools

- Language: C
- DB: MySQL 8.0
- IDE: VSCode

## Authors

- 김준서 (20200524)
