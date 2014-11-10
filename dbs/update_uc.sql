begin transaction;

insert into UserConf (Key, Value, ValueType) 
    select Key, DefaultValue, 0 from SysConf where key like "Debug.%" or Key="View.Threshold";

select * from UserConf;

commit;

