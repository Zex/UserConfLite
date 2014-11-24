<?php
/* d4config.php
 *
 * Author: Zex <top_zlynch@yahoo.com>
 */

include_once 'navi.php';

$CONFDB = "/home/ag/lab/Starter/dbs/config.db";
$SYSCONFTABLE = "SysConf";
$USERCONFTABLE = "UserConf";

//#define SQLITE_ERROR        1   /* SQL error or missing database */
//#define SQLITE_INTERNAL     2   /* Internal logic error in SQLite */
//#define SQLITE_PERM         3   /* Access permission denied */
//#define SQLITE_ABORT        4   /* Callback routine requested an abort */
//#define SQLITE_BUSY         5   /* The database file is locked */
//#define SQLITE_LOCKED       6   /* A table in the database is locked */
//#define SQLITE_NOMEM        7   /* A malloc() failed */
//#define SQLITE_READONLY     8   /* Attempt to write a readonly database */
//#define SQLITE_INTERRUPT    9   /* Operation terminated by sqlite3_interrupt()*/
//#define SQLITE_IOERR       10   /* Some kind of disk I/O error occurred */
//#define SQLITE_CORRUPT     11   /* The database disk image is malformed */
//#define SQLITE_NOTFOUND    12   /* Unknown opcode in sqlite3_file_control() */
//#define SQLITE_FULL        13   /* Insertion failed because database is full */
//#define SQLITE_CANTOPEN    14   /* Unable to open the database file */
//#define SQLITE_PROTOCOL    15   /* Database lock protocol error */
//#define SQLITE_EMPTY       16   /* Database is empty */
//#define SQLITE_SCHEMA      17   /* The database schema changed */
//#define SQLITE_TOOBIG      18   /* String or BLOB exceeds size limit */
//#define SQLITE_CONSTRAINT  19   /* Abort due to constraint violation */
//#define SQLITE_MISMATCH    20   /* Data type mismatch */
//#define SQLITE_MISUSE      21   /* Library used incorrectly */
//#define SQLITE_NOLFS       22   /* Uses OS features not supported on host */
//#define SQLITE_AUTH        23   /* Authorization denied */
//#define SQLITE_FORMAT      24   /* Auxiliary database format error */
//#define SQLITE_RANGE       25   /* 2nd parameter to sqlite3_bind out of range */
//#define SQLITE_NOTADB      26   /* File opened that is not a database file */
//#define SQLITE_ROW         100  /* sqlite3_step() has another row ready */
//#define SQLITE_DONE        101  /* sqlite3_step() has finished executing */

function update_configure()
{
    global $CONFDB;
    global $SYSCONFTABLE;
    global $USERCONFTABLE;

    if (!($conn = new SQLite3($CONFDB)))
    {
        throw new Exception($conn->lastErrorMsg(), $conn->lastErrorCode());
    }
    else
    {
        try
        {
            foreach ($_POST as $k => $v)
            {
                $item = ltrim(strrchr($k,'_'), '_');
                $key = rtrim(str_replace($item, "", $k), '_');
                $sql = "update ".$SYSCONFTABLE." set ".$item."=\"".$v."\" where Key == \"".str_replace('_', '.', $key)."\";";

                if (!($ret = $conn->query($sql)))
                    throw new Exception($conn->lastErrorMsg());
                
//                else
//                    echo "<div><span>"."Configure updated !"."</span></div>";
            }
        }
        catch (Exception $e)
        {
            echo "<div>".$e->getCode()." => ".$e->getMessage()."</div>";
        }
        
     }
    
     $conn->close();
}

function reset_user()
{
    global $CONFDB;
    global $SYSCONFTABLE;
    global $USERCONFTABLE;

    if (!($conn = new SQLite3($CONFDB)))
    {
        echo $conn->lastErrorMsg();
    }
    else
    {
        $sql = "update ".$USERCONFTABLE;
        $sql .= " set Value = (select ".$SYSCONFTABLE.".DefaultValue from ".$SYSCONFTABLE;
        $sql .= " where ".$USERCONFTABLE.".Key=".$SYSCONFTABLE.".Key)";
        $sql .= " where exists (select * from ".$SYSCONFTABLE." where ".$USERCONFTABLE.".Key=".$SYSCONFTABLE.".Key);";

        if (!($ret = $conn->query($sql)))
           echo "<div>".$conn->lastErrorMsg()."</div>";
            
        else
           echo "<div><span>"."Configure reseted !"."</span></div>";
     }
    
     $conn->close();
}

function read_sysconf()
{
    global $CONFDB;
    global $SYSCONFTABLE;
    global $USERCONFTABLE;
   

    if (!($conn = new SQLite3($CONFDB)))
        throw new Exception($conn->lastErrorMsg(), $conn->lastErrorCode());

    $titles = ["Key", "DefaultValue", "Step", "Upper", "Lower", "Unit"]; 

    $sql = "select ".join(",", $titles);
    $sql.rtrim(",");
    $sql .= " from ".$SYSCONFTABLE.";";
    
    if (!($ret = $conn->query($sql)))
    {
        echo $conn->lastErrorMsg();
    }
    else
    {
        echo "<form action=\"d4config?update\" method=\"post\" enctype=\"multipart/form-data\">";
        echo "<table class=\"normal\">";

        echo "<tr class=\"normal\">";
        foreach ($titles as $t)
            echo "<th class=\"normal\">".$t."</th>";
        echo "</tr>";

        while ($row = $ret->fetchArray(SQLITE3_ASSOC))
        {
            echo "<tr class=\"normal\">";
            echo "<td class=\"normal\">"."<span>".$row['Key']."</span>"."</td>";

            foreach ($titles as $t)
            {
                if ($t == 'Key') continue;

                echo "<td class=\"normal\">";
                echo "<input style=\"width:98%\" name=\"".$row['Key'].".".$t."\" type=\"text\" value=\"".$row[$t]."\"/>";
                echo "</td>";
            }

            echo "</tr>";
        }

        echo "</table>";
        echo "<input type=\"submit\" value=\""."Submit"."\"/>";
        echo "</form>";

        echo "<form action=\"d4config?additem\" method=\"post\" enctype=\"multipart/form-data\">";
        echo "<table class=\"normal\">";

        echo "<tr class=\"normal\">";
        foreach ($titles as $t)
        {
            echo "<td class=\"normal\">";
            echo "<input name=\"".$t."\" type=\"text\"/>";
            echo "</td>";
        }

        echo "</table>";
        echo "<input type=\"submit\" value=\""."New Item"."\"/>";
        echo "</form>";
    }

    $conn->close();
}

function read_userconf()
{
    global $CONFDB;
    global $SYSCONFTABLE;
    global $USERCONFTABLE;
   

    if (!($conn = new SQLite3($CONFDB)))
        echo $conn->lastErrorMsg();

    $titles = ["Key", "Value", "ValueType"];

    $sql = "select ".join(",", $titles);
    $sql.rtrim(",");
    $sql .= " from ".$USERCONFTABLE.";";

    if (!($ret = $conn->query($sql)))
    {
        throw new Exception($conn->lastErrorMsg(), $conn->lastErrorCode());
    }
    else
    {
        echo "<form action=\"d4config?reset\" method=\"post\" enctype=\"multipart/form-data\">";
        echo "<table class=\"normal\">";

        echo "<tr class=\"normal\">";
        foreach ($titles as $t)
            echo "<th class=\"normal\">".$t."</th>";
        echo "</tr>";

        while ($row = $ret->fetchArray(SQLITE3_ASSOC))
        {
            echo "<tr class=\"normal\">";
            echo "<form action=\"d4config?dui=".$row['Key']."\" method=\"post\" enctype=\"multipart/form-data\">";
            echo "<td class=\"normal\" width=\"70%\">";
            echo "<input type=\"submit\" name=\"".$row['Key']."\" value=\""."Delete"."\"/>";
            echo "<span>".$row['Key']."</span>"."</td>";
            echo "</form>";

            foreach ($titles as $t)
            {
                if ($t == 'Key') continue;

                echo "<td class=\"normal\" width=\"15%\">";
                echo "<input style=\"width:98%\" name=\"".$row['Key'].'.'.$t."\" type=\"text\" value=\"".$row[$t]."\" readonly=\"true\"/>";
                echo "</td>";
            }

            echo "</tr>";
        }

        echo "</table>";
        echo "<input type=\"submit\" value=\""."Reset"."\"/>";
        echo "</form>";

        echo "<form action=\"d4config?aui\" method=\"post\" enctype=\"multipart/form-data\">";
        echo "<table class=\"normal\">";

        echo "<tr class=\"normal\">";
        foreach ($titles as $t)
        {
            echo "<td class=\"normal\">";
            echo "<input name=\"".$t."\" type=\"text\"/>";
            echo "</td>";
        }

        echo "</table>";
        echo "<input type=\"submit\" value=\""."New Item"."\"/>";
        echo "</form>";
    }
    
    $conn->close();
}

function add_item()
{
    global $CONFDB;
    global $SYSCONFTABLE;
    global $USERCONFTABLE;

    if (!($conn = new SQLite3($CONFDB)))
    {
        throw new Exception($conn->lastErrorMsg(), $conn->lastErrorCode());
    }
    else
    {
        try
        {
            $req_keys = ["Key", "DefaultValue", "Step", "Upper", "Lower"];
            $sql = "insert into ".$SYSCONFTABLE." values (";

            foreach ($req_keys as $k)
            {
                $_POST[$k] = str_replace('"', "", $_POST[$k]);
                $_POST[$k] = str_replace(';', "", $_POST[$k]);
                $_POST[$k] = str_replace(' ', "", $_POST[$k]);

                if (empty($_POST[$k]))
                    throw new Exception("Empty key found", 1);
                $sql .= "\"".$_POST[$k]."\",";
            }

            $sql .= "\"".$_POST["Unit"]."\");";

            if (!($ret = $conn->query($sql)))
                throw new Exception($conn->lastErrorMsg(), $conn->lastErrorCode());

            $sql = "insert into ".$USERCONFTABLE." (Key, Value, ValueType)";
            $sql .= " select Key, DefaultValue, 0 from ".$SYSCONFTABLE." where Key=\"".$_POST["Key"]."\";";
             
//            echo "<div><span>".$sql."</span>"."</div>"; $conn->close();return;

            if (!($ret = $conn->query($sql)))
                throw new Exception($conn->lastErrorMsg(), $conn->lastErrorCode());
        }
        catch (Exception $e)
        {
            echo "<div>".$e->getCode()." => ".$e->getMessage()."</div>";
        }
        
     }
    
     $conn->close();
}

function add_user_item()
{
    global $CONFDB;
    global $SYSCONFTABLE;
    global $USERCONFTABLE;

    if (!($conn = new SQLite3($CONFDB)))
    {
        throw new Exception($conn->lastErrorMsg(), $conn->lastErrorCode());
    }
    else
    {
        try
        {
            $req_keys = ["Key", "Value"];

            $sql = "insert into ".$USERCONFTABLE." values (";

            foreach ($req_keys as $k)
            {
                $_POST[$k] = str_replace('"', "", $_POST[$k]);
                $_POST[$k] = str_replace(';', "", $_POST[$k]);
                $_POST[$k] = str_replace(' ', "", $_POST[$k]);

                if (empty($_POST[$k]))
                    throw new Exception("Empty key found", 1);
                $sql .= "\"".$_POST[$k]."\",";
            }

            $sql .= "\"".$_POST["ValueType"]."\");";

//            echo "<div><span>".$sql."</span>"."</div>"; $conn->close();return;

            if (!($ret = $conn->query($sql)))
                throw new Exception($conn->lastErrorMsg(), $conn->lastErrorCode());
        }
        catch (Exception $e)
        {
            echo "<div>".$e->getCode()." => ".$e->getMessage()."</div>";
        }
        
     }
    
     $conn->close();
}

function del_user_item($key)
{
    global $CONFDB;
    global $SYSCONFTABLE;
    global $USERCONFTABLE;

    if (!($conn = new SQLite3($CONFDB)))
    {
        throw new Exception($conn->lastErrorMsg(), $conn->lastErrorCode());
    }
    else
    {
        try
        {
            if (empty($key))
                throw new Exception("Empty key found", 1);

            $sql = "delete from ".$USERCONFTABLE." where Key=\"".$key."\";";

            if (!($ret = $conn->query($sql)))
                throw new Exception($conn->lastErrorMsg(), $conn->lastErrorCode());
        }
        catch (Exception $e)
        {
            echo "<div>".$e->getCode()." => ".$e->getMessage()."</div>";
        }
     }
    
     $conn->close();
}

function reply()
{

    header("Content-type: text/html; charset=utf-8");
    
    echo "<!DOCTYPE html>";
    echo "<html>";
    
    default_head("4D Config");
    
    echo "<body>";

    if ($_SERVER['REMOTE_ADDR'] == '192.168.0.205')
        default_navigator();

    echo "<div class=\"content\">";

    if (!empty($_POST))
    {
        $buf = parse_url($_SERVER['REQUEST_URI']);

//        foreach ($_SERVER as $a => $b)
//            echo "<div>".$a.' '.$b."</div>";
//        foreach ($buf as $a => $b)
//            echo "<div>".$a." => ".$b."</div>";

        try
        {
            $a = [];
            parse_str($buf['query'],$a);

            foreach ($a as $k => $v)
            {
                if ($k == 'update')
                    update_configure();
                else if ($k == 'reset')
                    reset_user();
                else if ($k == 'additem')
                    add_item();
                else if ($k == 'aui')
                    add_user_item();
                else if ($k == 'dui')
                    del_user_item($v);
            }
        }
        catch (Exception $e)
        {
            echo "<div>".$e->getCode()." => ".$e->getMessage()."</div>";
        }
    }

    echo "<h1>"."Configure for 3D/4D"."</h1>";

    echo "<h2>"."System Configure"."</h2>";
    read_sysconf();

    echo "<h2>"."User Configure"."</h2>";
    read_userconf();

    echo "</div>";
    echo "</body>";
    echo "</html>";
}

try
{
    reply();
}
catch (Exception $e)
{
    die ("d4config Error: ".$e->getCode()." => ".$e->getMessage());
}

?>
