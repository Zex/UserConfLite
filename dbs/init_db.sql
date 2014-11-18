/*
 * init_db.sql
 *
 * SQL sripts to initiate configure for 3D/4D
 *
 * Author: Zex <top_zlynch@yahoo.com>
 *
 * ValueType:
 * 0 => double
 * 1 => int
 * 2 => string
 *
 * Sample usage: sqlite3 ConfLite < init_db.sql
 */

begin transaction;

create table SysConf (
    Key text primary key not null,
    DefaultValue text not null 
    check (cast(DefaultValue as float) >= cast(Lower as float) and cast(DefaultValue as float) <= cast(Upper as float)),
    Step text not null,
    Upper text not null,
    Lower text not null,
    Unit text
);

                            /* Key,                 DefVal,  Step,   Upper,   Lower,  Unit */
insert into SysConf values ("Prepare.SwingAngle",   "70.0",  "10.0", "70.0",  "20.0", "");
insert into SysConf values ("Prepare.Distance",     "11.0",  "1.0",  "99.0",  "7.0", "cm");
insert into SysConf values ("Prepare.QualityIndex", "2.0",   "1.0",  "2.0",   "1.0",  "");
insert into SysConf values ("View.Vision",          "0.0",   "1.0",  "2.0",   "0.0",  "");
insert into SysConf values ("View.QRotate",         "0.0",   "90.0", "270.0", "0.0",  "");
insert into SysConf values ("View.RotationX",       "270.0", "5.0",  "360.0", "0.0",  "");
insert into SysConf values ("View.RotationY",       "0.0",   "5.0",  "360.0", "0.0",  "");
insert into SysConf values ("View.RotationZ",       "270.0", "5.0",  "360.0", "0.0",  "");
insert into SysConf values ("View.Chroma",          "2.0",   "1.0",  "7.0",   "0.0",  "");
insert into SysConf values ("View.Graymap",         "6.0",   "1.0",  "6.0",   "0.0",  "");
insert into SysConf values ("View.Zoom",            "1.0",   "0.4",  "2.2",   "0.6",  "");
insert into SysConf values ("View.Render",          "3.0",   "1.0",  "4.0",   "0.0",  "");
insert into SysConf values ("View.Threshold",       "50.0",  "5.0",  "100.0", "10.0", "");
insert into SysConf values ("Slice.Shift",          "0.0",   "0.1",  "1.0",   "0.0",  "");

insert into SysConf values ("Debug.Density.Vi0",    "1.0",   "0.1",  "1.0",   "0.0",  "");
insert into SysConf values ("Debug.Brightness.Vi0", "2.25",  "0.1",  "5.0",   "0.0",  "");
insert into SysConf values ("Debug.Contrast.Vi0",   "1.0",   "0.1",  "3.0",   "0.0",  "");
insert into SysConf values ("Debug.Smooth.Vi0",     "3.0",   "1.0",  "4.0",   "0.0",  "");

insert into SysConf values ("Debug.Density.Vi1",    "0.8",   "0.1",  "1.0",   "0.0",  "");
insert into SysConf values ("Debug.Brightness.Vi1", "2.05",  "0.1",  "5.0",   "0.0",  "");
insert into SysConf values ("Debug.Contrast.Vi1",   "1.0",   "0.1",  "3.0",   "0.0",  "");
insert into SysConf values ("Debug.Smooth.Vi1",     "3.0",   "1.0",  "4.0",   "0.0",  "");

insert into SysConf values ("Debug.Density.Vi2",    "0.4",   "0.1",  "1.0",   "0.0",  "");
insert into SysConf values ("Debug.Brightness.Vi2", "1.45",  "0.1",  "5.0",   "0.0",  "");
insert into SysConf values ("Debug.Contrast.Vi2",   "1.0",   "0.1",  "3.0",   "0.0",  "");
insert into SysConf values ("Debug.Smooth.Vi2",     "3.0",   "1.0",  "4.0",   "0.0",  "");

insert into SysConf values ("Review.Speed",         "0.0",   "1.0",  "3.0",   "0.0",  "");

create table UserConf (
    Key text primary key not null,
    Value text not null,
    ValueType integer not null
);

insert into UserConf (Key, Value, ValueType) 
    select Key, DefaultValue, 0 from SysConf;

insert into UserConf values ("Prepare.PreferredScan", "3D",    2);

commit;

                            /* Key,                  Value,   ValueType */
/*
insert into UserConf values ("Prepare.SwingAngle",   "60.0",  0);
insert into UserConf values ("Prepare.Distance", "11.0",  0);
insert into UserConf values ("Prepare.QualityIndex", "2.0",   0);
insert into UserConf values ("View.Vision",          "0.0",   0);
insert into UserConf values ("View.QRotate",         "0.0",   0);
insert into UserConf values ("View.RotationX",       "270.0", 0);
insert into UserConf values ("View.RotationY",       "0.0",   0);
insert into UserConf values ("View.RotationZ",       "270.0", 0);
insert into UserConf values ("View.Chroma",          "2.0",   0);
insert into UserConf values ("View.Graymap",         "3.0",   0);
insert into UserConf values ("View.Zoom",            "1.0",   0);
insert into UserConf values ("View.Render",          "3.0",   0);
insert into UserConf values ("Slice.Shift",          "0.0",   0);
insert into UserConf values ("Prepare.PreferedScan", "3D",    2);
*/
