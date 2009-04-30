drop table CUSTOMERS cascade constraints;
drop table SUBSCRIPTIONS cascade constraints;
drop table ENGINEERS cascade constraints;
drop table INSTALLATIONS cascade constraints;

create table Customers (
	hkid	char(8) primary key,
	name	char(30) not null,
	contact_phone_num	number(8) not null
);

insert into Customers values ('Z9228990', 'CHAN, Ko Hui', 67748874);
insert into Customers values ('K7828608', 'CHEUNG, Ka Fei', 98748763);
insert into Customers values ('G8900289', 'WONG, Lok', 32215372);
insert into Customers values ('H2289055', 'HUI, Man Kong', 36788421);
insert into Customers values ('Z6922899', 'CHAN, Ko Hui', 66778872);
insert into Customers values ('G9822892', 'MAN, Hui Yu', 37748908);

create table Subscriptions (
	sub_id	number(5) primary key,
	hkid	char(8) not null references Customers(hkid),
	installation_address	char(100) not null,
	district	char(3) not null check (district in ('HK', 'KLN', 'NT')),
	prefer_install_date	date not null
);

insert into Subscriptions values (10001, 'Z9228990', 'Flat 5A, Block E, Prime Garden, Tuen Mun', 'NT', to_date('20090422', 'yyyymmdd'));
insert into Subscriptions values (10002, 'K7828608', 'Room 2009, 20/F, Lung Lee House, Choi Hung', 'KLN', to_date('20090415', 'yyyymmdd'));
insert into Subscriptions values (10005, 'G8900289', 'Room 19B, Brilliant Town, Sheung Wan', 'HK', to_date('20090501', 'yyyymmdd'));
insert into Subscriptions values (10009, 'H2289055', 'Flat 2E, Block A, Best View Garden, Causeway Bay', 'HK', to_date('20090424', 'yyyymmdd'));
insert into Subscriptions values (10010, 'Z6922899', 'Room 1A, G/F, Kwong Lok House, Sha Tin', 'NT', to_date('20090424', 'yyyymmdd'));
insert into Subscriptions values (10021, 'G9822892', 'Flat J, Tower 1, Mong Tai Garden, North Point', 'HK', to_date('20090501', 'yyyymmdd'));
insert into Subscriptions values (10035, 'Z9228990', 'Flat 5B, Block E, Prime Garden, Tuen Mun', 'NT', to_date('20090422', 'yyyymmdd'));

create table Engineers (
	eng_id	number(3) primary key,
	name	char(30) not null,
	resp_district	char(3) not null check (resp_district in ('HK', 'KLN', 'NT'))
);

insert into Engineers values (222, 'CHAN, Ka Ho', 'HK');
insert into Engineers values (236, 'YU, Hau Kwong', 'KLN');
insert into Engineers values (242, 'YEUNG, William', 'KLN');
insert into Engineers values (255, 'AU YEUNG, Hon', 'NT');

create table Installations (
	task_id	number(6) primary key,
	sub_id	number(5) not null references Subscriptions(sub_id),
	eng_id	number(3) not null references Engineers(eng_id),
	Install_date	date
);

insert into Installations values ('300011', '10001', '255', to_date('20090422', 'yyyymmdd'));
insert into Installations values ('300012', '10005', '222', to_date('20090505', 'yyyymmdd'));
