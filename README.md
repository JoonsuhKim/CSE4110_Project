# Convenience Store DB Project

This project implements a supply-chain database system for a chain of convenience stores. It includes:

- Conceptual design (ERD)
- Relational schema in BCNF
- Physical schema in MySQL
- C application for query execution

## Development Environment

### OS & Hardware
- OS: macOS 13.7.5 (Build 22H527)

### Compiler & Build Tool
- Compiler: Apple Clang 14.0.3 (clang-1403.0.22.14.1)
- Target: x86_64-apple-darwin22.6.0

### Database Tools
- MySQL Server: 8.0.42
- MySQL Workbench: 8.0.42 CE (build 5076204)
- MySQL Connector C API: 8.0.42
  - Header Path: `/usr/local/mysql-8.0.42-macos15-x86_64/include`
  - Library Path: `/usr/local/mysql-8.0.42-macos15-x86_64/lib`

### IDE
- Visual Studio Code: 1.100.3 (x64)

### 과제 수행 과정
1. MySQL Workbench, MySQL, VSCode 및 Extension 설치
2. store 데이터베이스 생성
3. schema.sql <— 테이블 생성
4. 엑셀 파일로 샘플 데이터 생성 (일부 데이터 생성에서 LLM 활용)
5. SQL 쿼리로 변환 —> sample_data.sql
6. vscode로 스켈레톤 코드 확인 후 #include <mysql.h>로 헤더 경로 변경
7. main() 및 사용자정의 함수들 코딩
8. compile 확인 및 리포트 작성

### DB Setup
1. MySQL Workbench 실행
2. schema.sql 실행
3. sample_data.sql 실행
4. 컴파일 (아래 참고)

### Compile & Run Command
- 터미널 사용
- main.c 파일이 있는 경로로 접속
- 아래 명령어 실행

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
