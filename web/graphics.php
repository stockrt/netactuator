<?php

include "include/estilo.css";
include "include/connect.php";
include "include/functionLayout.php";
include "include/functions.php";
include "include/refresh.php";
//tipo de gráfico
$type = $_GET['type'];


if (!$result) {
    print "DB Error, não pude listar as tabelas\n";
    print 'MySQL Error: ' . mysql_error();
    exit;
}

if(!isset($host)) {

//	$sql = "SELECT host FROM storage_pattern_def";
	$sql = "SELECT DISTINCT host FROM storage_mass";
	$res = mysql_query($sql);
	$max = mysql_num_rows($res);
	
	for($i=0;$i < $max; $i++) {
		list($dados) = mysql_fetch_row($res);
		if(file_exists("graph/".$dados."_".$time."_".$type."_day.png"))
			print "<b><a href='graphics.php?host=".$dados."&type=".$type."'>".$dados."</a><br></b>";
		if(!file_exists("graph/".$dados."_".$time."_".$type."_day.png"))
				print $dados." - Gráfico não disponível<br>";
	}

} elseif (isset($host)){
	print "<center><b><font size='3'>$host - $type</font><br><br>";
	//print "Diário<br>";
	print "<img src='graph/".$host."_".$time."_".$type."_day.png'><br><br>";
	//print "Semanal<br>";
	print "<img src='graph/".$host."_".$time."_".$type."_week.png'><br><br>";
	//print "Mensal<br>";
	print "<img src='graph/".$host."_".$time."_".$type."_month.png'><br><br>";
	//print "Anual<br>";
	print "<img src='graph/".$host."_".$time."_".$type."_year.png'><br><br>";
} 

?>
