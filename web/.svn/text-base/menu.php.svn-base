<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Untitled Document</title>
<style type="text/css">
<!--
body {
	margin-left: 0px;
	margin-top: 0px;
	margin-right: 0px;
	margin-bottom: 0px;
}
body,td,th {
	font-family: menu;
}
-->
</style></head>

<body>
<table width="220" border="0" cellpadding="0" cellspacing="0">
  <!-- fwtable fwsrc="Untitled-1.png" fwbase="index.jpg" fwstyle="Dreamweaver" fwdocid = "338256838" fwnested="0" -->
  <tr>
    <td><img name="index_r2_c1" src="images/index_r2_c1.jpg" width="9" height="9" border="0" id="index_r2_c1" alt="" /></td>
    <td><img name="index_r2_c2" src="images/index_r2_c2.jpg" width="201" height="9" border="0" id="index_r2_c2" alt="" /></td>
    <td><img name="index_r2_c3" src="images/index_r2_c3.jpg" width="10" height="9" border="0" id="index_r2_c3" alt="" /></td>
  </tr>
  <tr>
     <td background="images/index_r3_c1.jpg"><img name="index_r3_c1" src="images/index_r3_c1.jpg" width="9" height="300" border="0" id="index_r3_c1" alt="" /></td>
    <td align="center" valign="top" background="images/index_r3_c2.jpg">

<?php
include "include/estilo.css";
include "include/connect.php";
include "include/functionLayout.php";
include "include/functions.php";
include "include/refresh.php";

 
if (!$result) {
    print "DB Error, não pude listar as tabelas\n";
    print 'MySQL Error: ' . mysql_error();
    exit;
}

$topTabelas = topTabelas();

while ($row = mysql_fetch_row($result)) {
	// pegar as propriedades das configurações de config_tables
	$checkConfigTables = checkConfigTables($row);
	// checar as visões das tabelas
	if(checkViewTables($row)) {
		// exibir tabelas
		$formTables = formTables($row, $description);
	}
}

$formGraphics = formGraphics();

print "</table>";

poweredby();

mysql_free_result($result); 
?>      &nbsp;</td>
    <td background="images/index_r3_c3.jpg"><img name="index_r3_c3" src="images/index_r3_c3.jpg" width="10" height="300" border="0" id="index_r3_c3" alt="" /></td>
  </tr>
  <tr>
    <td><img name="index_r4_c1" src="images/index_r4_c1.jpg" width="9" height="9" border="0" id="index_r4_c1" alt="" /></td>
    <td><img name="index_r4_c2" src="images/index_r4_c2.jpg" width="201" height="9" border="0" id="index_r4_c2" alt="" /></td>
    <td><img name="index_r4_c3" src="images/index_r4_c3.jpg" width="10" height="9" border="0" id="index_r4_c3" alt="" /></td>
  </tr>
</table>
</body>
</html>
