drop table vendor_request;
drop table vendor_storage;
drop table factory;
drop table purchase;
drop table store_request;
drop table store_storage;
drop table vendor;
drop table product;
drop table store;
drop table person;
drop table loyalty;

create table loyalty(
	grade	varchar(10) check (grade in ('basic', 'VIP')),
    dc_rate	varchar(5),
    primary key (grade)
    );

create table person(
	customer_id	varchar(10),
	name		varchar(20) not null,
    address 	varchar(20),
    level		varchar(10),
    phone_num	varchar(20) unique,
    primary key (customer_id),
    foreign key (level) references loyalty(grade)
    );

create table store(
	store_id	varchar(10),
    name		varchar(20) not null,
    address		varchar(20) not null,
    open		time,
    close		time,
    owner_type	varchar(10),
	phone_num	varchar(20) unique,
    primary key	(store_id)
    );

create table product (
	product_upc	varchar(10),
    name		varchar(20) not null unique,
    price		numeric(10,0) check (price>=0),
    type		varchar(10),
    brand		varchar(10),
    size		varchar(10),
    primary key	(product_upc)
    );
   
create table vendor (
	vendor_id	varchar(10),
	name		varchar(20) not null,
    address		varchar(20) not null,
    email		varchar(30) unique,
    primary key	(vendor_id)
    );
   
create table factory (
	factory_id	varchar(10),
    name		varchar(20) not null,
    address		varchar(20) not null,
    email		varchar(20) unique,
    primary key	(factory_id)
    );

create table store_storage (
	store_id	varchar(10),
    product_upc	varchar(10),
    inventory	numeric(5,0) not null check (inventory >= 0),
    threshold	numeric(5,0) not null check (threshold >= 0),
    reorder		numeric(5,0) not null,
    primary key (store_id, product_upc),
    foreign key (store_id)
		references store(store_id),
    foreign key (product_upc)
		references product(product_upc),
	check (reorder > threshold)
    );
   
create table store_request (
	request_id	varchar(10),
    date		date,
    shipping_mean	varchar(10),
    status		varchar(10) not null check (status in ('checking', 'shipping', 'delivered')),
    store_id	varchar(10),
    product_upc	varchar(10),
    vendor_id	varchar(10),
    primary key	(request_id),
    foreign key (store_id)
		references store(store_id)
        on delete set null,
    foreign key (product_upc)
		references product(product_upc)
        on delete set null,
    foreign key (vendor_id)
		references vendor(vendor_id)
        on delete set null
-- 오류 가능성으로 복합 외래키는 제거
    );

create table vendor_storage (
	vendor_id	varchar(10),
    product_upc	varchar(10),
    inventory	numeric(5,0) not null check (inventory >= 0),
    threshold	numeric(5,0) not null check (threshold >= 0),
    reorder		numeric(5,0) not null,
    primary key (vendor_id, product_upc),
    foreign key (vendor_id)
		references vendor(vendor_id),
    foreign key (product_upc)
		references product(product_upc),
	check (reorder > threshold)
	);
    
create table vendor_request (
	request_id	varchar(10),
    date		date,
    shipping_mean	varchar(10),
    status		varchar(10) not null check (status in ('checking', 'shipping', 'delivered')),
    vendor_id	varchar(10),
    product_upc	varchar(10),
    factory_id	varchar(10),
    primary key	(request_id),
    foreign key (vendor_id)
		references vendor(vendor_id)
        on delete set null,
    foreign key (product_upc)
		references product(product_upc)
        on delete set null,
    foreign key (factory_id)
		references factory(factory_id)
        on delete set null
-- 오류 가능성으로 복합 외래키는 제거
    );
   
create table purchase (
	purchase_id	varchar(10),
    date		date not null,
    quantity	numeric(5,0) not null check (quantity > 0),
    method		varchar(5) check (method in ('card', 'cash')),
    customer_id	varchar(10),
    store_id	varchar(10),
    product_upc	varchar(10),
    primary key (purchase_id),
    foreign key	(customer_id)
		references person(customer_id)
		on delete set null,
	foreign key	(store_id)
		references store(store_id)
		on delete set null,
	foreign key	(product_upc)
		references product(product_upc)
		on delete set null
-- 오류 가능성으로 복합 외래키는 제거
	);