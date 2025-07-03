#include <stdio.h>
#include <string.h>
#include <mysql.h>

void ProductAvailability(MYSQL *conn);
void Top_SellingItems(MYSQL *conn);
void StorePerformance(MYSQL *conn);
void VendorStatistics(MYSQL *conn);
void InventoryAlerts(MYSQL *conn);
void CustomerPatterns(MYSQL *conn);
void FranchiseComparison(MYSQL *conn);
void ExitProgram(MYSQL *conn);
void PrintQueryResult(MYSQL_RES *res);

int main() {
    MYSQL *conn;

    const char *server = "localhost";
    const char *user = "root";
    const char *password = "1234"; // 여기에 비밀번호 입력
    const char *database = "store"; // 여기에 데이터베이스 이름 입력

    // MySQL 초기화
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        return 1;
    }

    // MySQL 서버 연결
    if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed\n");
        mysql_close(conn);
        return 1;
    }

    int slct;
    while(1){
        printf("---------- SELECT QUERY TYPE ----------\n\n");
        printf("    1. TYPE 1\n");
        printf("    2. TYPE 2\n");
        printf("    3. TYPE 3\n");
        printf("    4. TYPE 4\n");
        printf("    5. TYPE 5\n");
        printf("    6. TYPE 6\n");
        printf("    7. TYPE 7\n");
        printf("    0. QUIT\n\n");
        printf("SELECT: ");
        scanf("%d", &slct);

        switch (slct){
            case 1: ProductAvailability(conn); break;
            case 2: Top_SellingItems(conn); break;
            case 3: StorePerformance(conn); break;
            case 4: VendorStatistics(conn); break;
            case 5: InventoryAlerts(conn); break;
            case 6: CustomerPatterns(conn); break;
            case 7: FranchiseComparison(conn); break;
            case 0: ExitProgram(conn); break;
            default: printf("Invalid Input\n"); continue;
        }

        printf("\n");
    }

    // 리소스 해제
    mysql_close(conn);

    return 0;
}

void ProductAvailability(MYSQL *conn) {
    MYSQL_RES *res;
    MYSQL_ROW row;
    
    char query[256];
    char prdct[10];
    printf("\n---------- TYPE 1 ----------\n");
    printf("Which stores currently carry a certain product (by UPC, name, or brand), and how much inventory do they have?\n");
    printf("Enter a product_upc: ");
    scanf("%s", prdct);
    sprintf(query, ""
        "SELECT st.name, ss.product_upc, ss.inventory "
        "FROM store_storage ss NATURAL JOIN store st "
        "WHERE ss.product_upc = '%s'", prdct
    );

    // 쿼리 실행
    if (mysql_query(conn, query)) {
        fprintf(stderr, "SELECT failed. Error: %s\n", mysql_error(conn));
        return;
    }    

    // 결과 저장
    res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "mysql_store_result() failed. Error: %s\n", mysql_error(conn));
        return;
    }

    // 리소스 해제
    PrintQueryResult(res);
    mysql_free_result(res);
}

void Top_SellingItems(MYSQL *conn) {
    MYSQL_RES *res;
    MYSQL_ROW row;

    char query[1024];
    printf("\n---------- TYPE 2 ----------\n");
    printf("Which products have the highest sales volume in each store over the past month?\n");
    sprintf(query, ""
    "WITH monthly_data AS ("
    "SELECT * "
    "FROM purchase "
    "WHERE date >= '2025-05-01' AND date <= '2025-05-31' "
    "), "
    "monthly_total AS ("
    "SELECT store_id, product_upc, SUM(quantity) AS sales "
    "FROM monthly_data "
    "GROUP BY store_id, product_upc"
    "), "
    "store_max_sales AS ("
    "SELECT store_id, MAX(sales) AS m_sales "
    "FROM monthly_total "
    "GROUP BY store_id"
    ") "
    "SELECT st.name, pd.name "
    "FROM monthly_total AS mt NATURAL JOIN store st "
    "JOIN product pd ON mt.product_upc = pd.product_upc "
    "JOIN store_max_sales AS sms "
    "ON mt.store_id = sms.store_id AND mt.sales = sms.m_sales;"
    );

    // 쿼리 실행
    if (mysql_query(conn, query)) {
        fprintf(stderr, "SELECT failed. Error: %s\n", mysql_error(conn));
        return;
    }    

    // 결과 저장
    res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "mysql_store_result() failed. Error: %s\n", mysql_error(conn));
        return;
    }

    // 리소스 해제
    PrintQueryResult(res);
    mysql_free_result(res);
}

void StorePerformance(MYSQL *conn) {
    MYSQL_RES *res;
    MYSQL_ROW row;

    char query[1024];
    printf("\n---------- TYPE 3 ----------\n");
    printf("Which store has generated the highest overall revenue this quarter?\n");
    sprintf(query, ""
    "WITH quarter_data as ("
    "SELECT store_id, product_upc, quantity "
    "FROM purchase "
    "WHERE date >= '2025-04-01' AND date <= '2025-06-30'), "
    "store_revenue as ("
    "SELECT store_id, SUM(qd.quantity * pd.price) as revenue "
    "FROM quarter_data as qd JOIN product as pd "
    "ON qd.product_upc = pd.product_upc "
    "GROUP BY store_id) "
    "SELECT st.name "
    "FROM store_revenue as sr NATURAL JOIN store st "
    "WHERE sr.revenue = (SELECT MAX(revenue) "
    "FROM store_revenue);"
    );

        // 쿼리 실행
    if (mysql_query(conn, query)) {
        fprintf(stderr, "SELECT failed. Error: %s\n", mysql_error(conn));
        return;
    }    

    // 결과 저장
    res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "mysql_store_result() failed. Error: %s\n", mysql_error(conn));
        return;
    }

    // 리소스 해제
    PrintQueryResult(res);
    mysql_free_result(res);
}

void VendorStatistics(MYSQL *conn) {
    MYSQL_RES *res;
    MYSQL_ROW row;

    char query[1024];
    printf("\n---------- TYPE 4 ----------\n");
    printf("Which vendor supplies the most products across the chain, and how many total units have been sold?\n"); 
    sprintf(query, ""
    "WITH vendor_total as ("
    "SELECT vendor_id, store_id, product_upc "
    "FROM store_request "
    "WHERE status = 'delivered'), "
    "vendor_q as ("
    "SELECT vt.vendor_id, SUM(ss.reorder) as total_supply "
    "FROM vendor_total as vt JOIN store_storage as ss "
    "ON vt.store_id = ss.store_id AND vt.product_upc = ss.product_upc "
    "GROUP BY vt.vendor_id) "
    "SELECT vd.name, vq.total_supply "
    "FROM vendor_q vq NATURAL JOIN vendor vd "
    "WHERE total_supply = (SELECT MAX(total_supply) "
    "FROM vendor_q);"
    );
    
    // 쿼리 실행
    if (mysql_query(conn, query)) {
        fprintf(stderr, "SELECT failed. Error: %s\n", mysql_error(conn));
        return;
    }    

    // 결과 저장
    res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "mysql_store_result() failed. Error: %s\n", mysql_error(conn));
        return;
    }

    // 리소스 해제
    PrintQueryResult(res);
    mysql_free_result(res);    
}

void InventoryAlerts(MYSQL *conn) {
    MYSQL_RES *res;
    MYSQL_ROW row;

    char query[1024];
    printf("\n---------- TYPE 5 ----------\n");
    printf("Which products in each store are below the reorder threshold and need restocking?\n");
    sprintf(query, ""
    "SELECT st.name, pd.name "
    "FROM store_storage ss NATURAL JOIN store st "
    "JOIN product pd ON ss.product_upc = pd.product_upc "
    "WHERE inventory < threshold;"
    );

    // 쿼리 실행
    if (mysql_query(conn, query)) {
        fprintf(stderr, "SELECT failed. Error: %s\n", mysql_error(conn));
        return;
    }    

    // 결과 저장
    res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "mysql_store_result() failed. Error: %s\n", mysql_error(conn));
        return;
    }

    // 리소스 해제
    PrintQueryResult(res);
    mysql_free_result(res);
}

void CustomerPatterns(MYSQL *conn) {
    MYSQL_RES *res;
    MYSQL_ROW row;

    char query[1024];
    char prdct[10];
    printf("\n---------- TYPE 6 ----------\n");
    printf("List the top 3 items that loyalty program customers typically purchase with coffee.\n");
    printf("Enter a product type 'coffee': ");
    scanf("%s", prdct);
    sprintf(query, ""
    "WITH p_code as ("
    "SELECT product_upc "
    "FROM product "
    "WHERE type = '%s'), "
    "accompanied_data as ("
    "SELECT pc.date as date, pc.customer_id as customer_id, pc.store_id as store_id "
    "FROM purchase as pc JOIN p_code as pcd "
    "ON pc.product_upc = pcd.product_upc "
    "JOIN person as ps "
    "ON pc.customer_id = ps.customer_id "
    "WHERE ps.level = 'VIP'), "
    "accompanied_product as ("
    "SELECT pc.product_upc as product_upc, SUM(pc.quantity) as quantity "
    "FROM purchase as pc JOIN accompanied_data as ad "
    "ON pc.date = ad.date "
    "AND pc.customer_id = ad.customer_id "
    "AND pc.store_id = ad.store_id "
    "WHERE pc.product_upc NOT IN (SELECT product_upc FROM p_code) "
    "GROUP BY pc.product_upc) "
    "SELECT pd.name, ap.quantity "
    "FROM accompanied_product as ap JOIN product as pd "
    "ON ap.product_upc = pd.product_upc "
    "ORDER BY quantity DESC "
    "LIMIT 3;", prdct);

    // 쿼리 실행
    if (mysql_query(conn, query)) {
        fprintf(stderr, "SELECT failed. Error: %s\n", mysql_error(conn));
        return;
    }    

    // 결과 저장
    res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "mysql_store_result() failed. Error: %s\n", mysql_error(conn));
        return;
    }

    // 리소스 해제
    PrintQueryResult(res);
    mysql_free_result(res);  
}

void FranchiseComparison(MYSQL *conn) {
    MYSQL_RES *res;
    MYSQL_ROW row;

    char query[1024];
    printf("\n---------- TYPE 7 ----------\n");
    printf("Among franchise-owned stores, which one offers the widest variety of products, and how does that compare to corporate-owned stores?\n");
    printf("franchise store with the widest product variety is: \n");
    sprintf(query, ""
    "WITH franchise_list as ("
    "SELECT store_id "
    "FROM store "
    "WHERE owner_type = 'franchise'), "
    "franchise_pnum as ("
    "SELECT fl.store_id, COUNT(ss.product_upc) as num "
    "FROM franchise_list as fl JOIN store_storage as ss "
    "ON fl.store_id = ss.store_id "
    "GROUP BY fl.store_id) "
    "SELECT st.name, fp.num "
    "FROM franchise_pnum fp NATURAL JOIN store st "
    "WHERE fp.num = (SELECT MAX(num) "
    "FROM franchise_pnum);"
    );

    // 쿼리 실행
    if (mysql_query(conn, query)) {
        fprintf(stderr, "SELECT failed. Error: %s\n", mysql_error(conn));
        return;
    }    

    // 결과 저장
    res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "mysql_store_result() failed. Error: %s\n", mysql_error(conn));
        return;
    }

    // 리소스 해제
    PrintQueryResult(res);

    printf("\ncorporate stores have following variety of products: \n");
    sprintf(query, ""
    "WITH corporate_list as ("
    "SELECT store_id "
    "FROM store "
    "WHERE owner_type = 'corporate') "
    "SELECT st.name, COUNT(ss.product_upc) as num "
    "FROM corporate_list as cl NATURAL JOIN store st "
    "JOIN store_storage as ss "
    "ON cl.store_id = ss.store_id "
    "GROUP BY cl.store_id;"
    );

    // 쿼리 실행
    if (mysql_query(conn, query)) {
        fprintf(stderr, "SELECT failed. Error: %s\n", mysql_error(conn));
        return;
    }    

    // 결과 저장
    res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "mysql_store_result() failed. Error: %s\n", mysql_error(conn));
        return;
    }

    // 리소스 해제
    PrintQueryResult(res);
    mysql_free_result(res);
}

void PrintQueryResult(MYSQL_RES *res) {
    MYSQL_ROW row;

    // 필드 개수 가져오기
    int num_fields = mysql_num_fields(res);
    MYSQL_FIELD *fields = mysql_fetch_fields(res);

    // 헤더 출력
    for (int i = 0; i < num_fields; i++) {
        printf("%s\t", fields[i].name);
    }
    printf("\n");

    // 행 출력
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            printf("%s\t", row[i] ? row[i] : "NULL");
        }
        printf("\n");
    }
}

void ExitProgram(MYSQL *conn) {
    printf("Program terminated.\n");
    mysql_close(conn);
    exit(0);
}