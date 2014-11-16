<?php
/* d4config.php
 *
 * Author: Zex <top_zlynch@yahoo.com>
 */

include_once 'navi.php';

$CONFDB = "/home/ag/lab/Starter/dbs/config.db";
$SYSCONFTABLE = "SysConf";
$USERCONFTABLE = "UserConf";

function update_configure()
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
            echo "<div>".$conn->lastErrorMsg()."</div>";
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
        echo $conn->lastErrorMsg();

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
                echo "<input name=\"".$row['Key'].".".$t."\" type=\"text\" value=\"".$row[$t]."\"/>";
                echo "</td>";
            }

            echo "</tr>";
        }

        echo "</table>";
        echo "<input type=\"submit\" value=\""."Submit"."\"/>";
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

    $titles = ["Key", "Value"];

    $sql = "select ".join(",", $titles);
    $sql.rtrim(",");
    $sql .= " from ".$USERCONFTABLE.";";

    if (!($ret = $conn->query($sql)))
    {
        echo $conn->lastErrorMsg();
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
            echo "<td class=\"normal\">"."<span>".$row['Key']."</span>"."</td>";

            foreach ($titles as $t)
            {
                if ($t == 'Key') continue;

                echo "<td class=\"normal\">";
                echo "<input name=\"".$row['Key'].'.'.$t."\" type=\"text\" value=\"".$row[$t]."\" readonly=\"true\"/>";
                echo "</td>";
            }

            echo "</tr>";
        }

        echo "</table>";
        echo "<input type=\"submit\" value=\""."Reset"."\"/>";
        echo "</form>";

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
            if ($buf['query'] == 'update')
                update_configure();
    
            else if ($buf['query'] == 'reset')
                reset_user();
        }
        catch (Exception $e)
        {
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
    die ('d4config Error: ' . $e->getMessage());
}

?>
