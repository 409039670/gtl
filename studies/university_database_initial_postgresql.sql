
drop table SC;
drop table TC;
drop table Course;
drop table Teacher;
drop table Student;
drop table Department; 

/*ϵ����Ϣ�� Department��Dno,Dname,Daddress��*/
create table Department(
	Dno int,
	Dname varchar(50),
	Daddress varchar(50),
	primary key (Dno)
	);
insert into Department(Dno,Dname,Daddress) values(1,'�����ѧѧԺ','��¥��');
insert into Department(Dno,Dname,Daddress) values(2,'��ԴѧԺ','��¥��');
insert into Department(Dno,Dname,Daddress) values(3,'�Ļ�ѧԺ','�Ļ�¥');
insert into Department(Dno,Dname,Daddress) values(4,'����ѧԺ','�Ļ�¥');
insert into Department(Dno,Dname,Daddress) values(5,'����ѧԺ','ˮ��¥');
insert into Department(Dno,Dname,Daddress) values(6,'����������ռ���ϢѧԺ','��̽¥');
insert into Department(Dno,Dname,Daddress) values(7,'��е�������ϢѧԺ','�̶�¥');
insert into Department(Dno,Dname,Daddress) values(8,'���ù���ѧԺ','����¥');
insert into Department(Dno,Dname,Daddress) values(9,'����ѧԺ','��һ¥');
insert into Department(Dno,Dname) values(10,'��Ϣ����ѧԺ');
insert into Department(Dno,Dname,Daddress) values(11,'��ѧ������ѧԺ','��ί¥');
insert into Department(Dno,Dname,Daddress) values(12,'�鱦ѧԺ','�鱦¥');
insert into Department(Dno,Dname,Daddress) values(13,'����ѧԺ','����¥');
insert into Department(Dno,Dname,Daddress) values(14,'�����ѧԺ','��һ¥');
insert into Department(Dno,Dname) values(15,'Զ�����������ѧԺ');
insert into Department(Dno,Dname) values(16,'���ʽ���ѧԺ');
insert into Department(Dno,Dname,Daddress) values(17,'������','������');
insert into Department(Dno,Dname,Daddress) values(18,'�����봫ýѧԺ','�մ�¥');
insert into Department(Dno,Dname,Daddress) values(19,'���˼����ѧԺ','����¥');
insert into Department(Dno,Dname,Daddress) values(20,'����ѧԺ','����У��');

/*ѧ����Ϣ��Student��Sno, Sname, Ssex, Sage, Dno��*/
create table Student(
	Sno char(11) ,
	Sname varchar(50),
	Ssex char(2),
	Sage int,
	Dno int,
	primary key (Sno),
	foreign key (Dno) references Department(Dno)
	);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20091000231','����','M',18,14);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20091004391','����','M',19,14);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20091001598','������','M',20,14);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20091003085','Ԭ��','M',18,14);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20091000863','�Ϸ�','M',17,14);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20091000934','����','M',19,8);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20091000961','��ѩ��','F',18,8);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20091000983','����','M',19,8);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20091001175','����','M',18,8);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20091001261','��','M',17,8);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20091001384','����','M',20,8);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20081003492','�׼���','M',19,5);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20081001197','���Ӵ�','M',17,5);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20081001266','�̾�ѧ','F',19,5);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20081001888','������','F',20,5);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20091000481','����','M',17,5);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20101000199','����Т','M',18,11); 
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20101000424','���','M',17,11);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20101000481','����ǿ','M',16,11);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20101000619','�²�','M',20,11);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20101000705','������','M',18,11);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20101000802','�պ���','M',17,11);


/*��ʦ��Ϣ��Teacher ��Tno, Tname, Ttitle, Dno��*/
create table Teacher(
	Tno int primary key,
	Tname varchar(50) ,
	Ttitle varchar(50),
	Dno int ,
	foreign key (Dno) references Department(Dno)
	);
insert into Teacher(Tno, Tname, Ttitle, Dno) values(1,'��С��','������',14);
insert into Teacher(Tno, Tname, Ttitle, Dno) values(2,'���ղ�','����',14);
insert into Teacher(Tno, Tname, Ttitle, Dno) values(3,'������','����',11);
insert into Teacher(Tno, Tname, Ttitle, Dno) values(4,'��ƽ��','��ʦ',14);
insert into Teacher(Tno, Tname, Ttitle, Dno) values(5,'���','��ʦ',14);
insert into Teacher(Tno, Tname, Ttitle, Dno) values(6,'������','��ʦ',14);
insert into Teacher(Tno, Tname, Ttitle, Dno) values(7,'���ķ�','������',14);

/*�γ���Ϣ�� Course  ��Cno, Cname, Cpno, Ccredit)*/
create table Course(
	Cno int primary key ,
	Cname varchar(50),
	Cpno int ,
	CCredit int,
	foreign key(Cpno) references Course(Cno)
	);
insert into Course(Cno, Cname, Ccredit) values(2,'�ߵ���ѧ',8);
insert into Course(Cno, Cname, Ccredit) values(6,'C���Գ������',4);
insert into Course(Cno, Cname, Ccredit) values(7,'��ѧ����',8);
insert into Course(Cno, Cname, Ccredit) values(8,'��ѧ��ѧ',3);
insert into Course(Cno, Cname, Ccredit) values(10,'�������',2);
insert into Course(Cno, Cname, Ccredit) values(12,'������ʷ',2);
insert into Course(Cno, Cname, Ccredit) values(13,'�й�ͨʷ',6);
insert into Course(Cno, Cname, Ccredit) values(14,'��ѧ����',3);
insert into Course(Cno, Cname, Cpno, Ccredit) values(5,'���ݽṹ',6,4);
insert into Course(Cno, Cname, Cpno, Ccredit) values(4,'����ϵͳ',5,4);
insert into Course(Cno, Cname, Cpno, Ccredit) values(1,'���ݿ�ԭ��',5,4);
insert into Course(Cno, Cname, Cpno, Ccredit) values(3,'��Ϣϵͳ',1,2);
insert into Course(Cno, Cname, Cpno, Ccredit) values(9,'�������',6,2);
insert into Course(Cno, Cname, Cpno, Ccredit) values(11,'�ռ����ݿ�',1,3);


/*ѧ��ѡ�α�SC��Sno,Cno,Grade��*/
create table SC(
	Sno char(11) ,
	Cno int, 
	Grade int,
	primary key(Sno, Cno),
	foreign key(Sno) references Student(Sno),
	foreign key (Cno) references Course(Cno)
	);
insert into SC values('20091003085',1,90);
insert into SC values('20091000863',1,98);
insert into SC values('20091000934',1,89);
insert into SC values('20091000961',1,85);
insert into SC values('20081001197',1,79);
insert into SC values('20081001266',1,97);
insert into SC values('20081001888',1,60);
insert into SC values('20091000481',1,78);
insert into SC values('20101000199',1,65);
insert into SC values('20101000424',1,78);
insert into SC values('20101000481',1,69);
insert into SC values('20091000863',6,90);
insert into SC values('20091000934',6,90);
insert into SC values('20091000961',6,87); 
 
/*��ʦ�ڿα�TC��Tno,Cno,Site��*/
create table TC(
	Tno int ,
	Cno int,
	Site varchar(50),
	primary key (Tno,Cno),
	foreign key(Tno) references Teacher(Tno),
	foreign key (Cno) references Course(Cno)
	);
insert into TC values(1,1,'��һ¥407');
insert into TC values(1,6,'��һ¥307');
insert into TC values(2,10,'�̶�¥217');
insert into TC values(3,2,'����¥507');
insert into TC values(4,5,'����¥208');
insert into TC values(6,3,'�ۺ�¥207');
insert into TC values(7,4,'�̶�¥817');
insert into TC values(5,9,'��һ¥207');