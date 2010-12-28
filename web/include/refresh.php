<?php

$sql_time = "SELECT flow_capture_time_min FROM config_netactuator";
$res_time = mysql_query($sql_time);
$time = mysql_result($res_time, 0);
$time = $time*60;

print "<META HTTP-EQUIV='Refresh' CONTENT='$time'>";

?>
