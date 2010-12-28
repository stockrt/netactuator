<?php

//$host = "locahost";
//$user = "root";
//$pass = "";
// $dbname = "netactuator";
// $conecta = mysql_connect($host, $user, $pass, $dbname);

$connect = mysql_connect("localhost", "netactuator", "netdbpass", "netactuator");
$result = mysql_list_tables("netactuator");
?>
