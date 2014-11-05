/*
 * ValueType:
 * 0 double
 * 1 int
 * 2 string
 *
create table UserConf (
Key text primary key not null,
Value text not null,
ValueType integer not null
);

insert into UserConf values ("Prepare.SwingAngle", "60.0", 0);
insert into UserConf values ("Prepare.MovingLength", "11.0", 0);
insert into UserConf values ("Prepare.QualityIndex", "2.0", 0);
insert into UserConf values ("View.Vision", "0.0", 0);
insert into UserConf values ("View.QRotate", "0.0", 0);
insert into UserConf values ("View.RotationX", "270.0", 0);
insert into UserConf values ("View.RotationY", "0.0", 0);
insert into UserConf values ("View.RotationZ", "270.0", 0);
insert into UserConf values ("View.Chroma", "2.0", 0);
insert into UserConf values ("View.Graymap", "3.0", 0);
insert into UserConf values ("View.Zoom", "1.0", 0);
insert into UserConf values ("View.Render", "3.0", 0);
insert into UserConf values ("Slice.Shift", "0.0", 0);
insert into UserConf values ("Prepare.PreferedScan", "3D", 2);

 */

