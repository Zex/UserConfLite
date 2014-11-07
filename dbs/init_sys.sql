/*
 * Initial script for system configure
 */
create table SysConf (
    Key text primary key not null,
    DefaultValue text not null,
    Step text not null,
    Upper text not null,
    Lower text not null,
    Unit text
);

insert into SysConf values ("Prepare.SwingAngle", 60.0, 2.0, 30.0, 1.0, "");
insert into SysConf values ("Prepare.MovingLength", 11.0, 2.0, 30.0, 1.0, "cm");
insert into SysConf values ("Prepare.QualityIndex", 2.0, 2.0, 30.0, 1.0, "");
insert into SysConf values ("View.Vision", 0.0, 2.0, 30.0, 1.0, "");
insert into SysConf values ("View.QRotate", 0.0, 2.0, 30.0, 1.0, "");
insert into SysConf values ("View.RotationX", 270.0, 2.0, 30.0, 1.0, "");
insert into SysConf values ("View.RotationY", 0.0, 2.0, 30.0, 1.0, "");
insert into SysConf values ("View.RotationZ", 270.0, 2.0, 30.0, 1.0, "");
insert into SysConf values ("View.Chroma", 2.0, 2.0, 30.0, 1.0, "");
insert into SysConf values ("View.Graymap", 3.0, 2.0, 30.0, 1.0, "");
insert into SysConf values ("View.Zoom", 1.0, 2.0, 30.0, 1.0, "");
insert into SysConf values ("View.Render", 3.0, 2.0, 30.0, 1.0, "");
insert into SysConf values ("Slice.Shift", 0.0, 2.0, 30.0, 1.0, "");


