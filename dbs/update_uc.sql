begin transaction;
/*
insert into UserConf (Key, Value, ValueType) 
    select Key, DefaultValue, 0 from SysConf where key like "Debug.%" or Key="View.Threshold";
*/

/*
update UserConf set Key="Prepare.Distance" where Key="Prepare.MovingLength";
*/

select * from UserConf;

commit;

