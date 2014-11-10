begin transaction;
/*
insert into UserConf (Key, Value, ValueType) 
    select Key, DefaultValue, 0 from SysConf where key like "Debug.%" or Key="View.Threshold";
*/

/*
update UserConf set Key="Prepare.Distance" where Key="Prepare.MovingLength";
*/

update UserConf
set Value = (select SysConf.DefaultValue from SysConf where UserConf.Key=SysConf.Key)
where exists (select * from SysConf where UserConf.Key=SysConf.Key);

select Key, '-',  Value, '-', ValueType from UserConf;

commit;

