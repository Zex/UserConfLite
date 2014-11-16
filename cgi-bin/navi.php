<?php

function default_head($title)
{
    echo "<head>";
    echo "<title>".$title."</title>";
    echo "<link href=\"/css/basic.css\" rel=\"stylesheet\" type=\"text/css\">";
    echo "<link href=\"/img/badsmile.jpg\" rel=\"icon\" type=\"image/jpg\">";
    echo "<meta charset=\"UTF-8\">";
    echo "</head>";
}

function default_navigator()
{
    echo "<div class=\"navigator\">";
    echo "<a name=\"Navigator\"><ul>Navigator</ul></a>";
    echo "<ul>";
    echo "<li><a href=\"starter.py#YouOut\" title=\"You Out\">You Out</a></li>";
    echo "<li><a href=\"starter.py#Motions\" title=\"Motions\">Motions</a></li>";
    echo "<li><a href=\"starter.py#ConnSVN\" title=\"ConnSVN\">Connect to SVN</a></li>";
    echo "<li><a href=\"starter.py#RandomSeq\" title=\"Random Seq\">Random Seq</a></li>";
    echo "<li><a href=\"starter.py#LeaveMessage\" title=\"Leave a Message\">Leave a Message</a></li>";
    echo "</ul>";
    echo "</div>";
}

?>
