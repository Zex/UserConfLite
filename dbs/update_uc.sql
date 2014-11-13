.separator ' '

begin transaction;
/*
insert into UserConf (Key, Value, ValueType) 
    select Key, DefaultValue, 0 from SysConf where key like "Debug.%" or Key="View.Threshold";
*/

/*
update UserConf set Key="Prepare.Distance" where Key="Prepare.MovingLength";
*/
/*
update UserConf set Value="100.0" where Key="View.Threshold";
update UserConf set Value="111.0" where Key="Prepare.Distance";

update UserConf
set Value = (select SysConf.DefaultValue from SysConf where UserConf.Key=SysConf.Key and SysConf.Key like "View.%")
where exists (select * from SysConf where UserConf.Key=SysConf.Key and SysConf.Key like "View.%");
*/
update UserConf
set Value = (select SysConf.DefaultValue from SysConf where UserConf.Key=SysConf.Key)
where exists (select * from SysConf where UserConf.Key=SysConf.Key);

/*
select Key, ',Value: ',  Value, ',Type: ', ValueType from UserConf;
select Key, ',DefaultValue: ', DefaultValue, ',Step: ', Step, ',Upper: ', Upper, ',Lower: ', Lower from SysConf;

alter table SysConf
add chk_defval
check (cast(DefaultValue as float) >= cast(Lower as float) and cast(DefaultValue as float) <= cast(Upper as float));
*/

/*
select cast(round(0.3, 2) as float);
select cast(round(0.3, 2) as int);

select Key, ',Value: ',  cast(Value as float), ',Type: ', ValueType from UserConf;
*/
/*
select Key,
',DefaultValue: ', cast(DefaultValue as float),
',Step: ', cast(Step as float),
',Upper: ', cast(Upper as float),
',Lower: ', cast(Lower as float) from SysConf;
*/
commit;
