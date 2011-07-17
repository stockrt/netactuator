<?php
$diasemana[0] = "Sunday";
$diasemana[1] = "Monday";
$diasemana[2] = "Tuesday";
$diasemana[3] = "Wednesday";
$diasemana[4] = "Thursday";
$diasemana[5] = "Friday";
$diasemana[6] = "Saturday";

$mesnome[1] = "January";
$mesnome[2] = "February";
$mesnome[3] = "March";
$mesnome[4] = "April";
$mesnome[5] = "May";
$mesnome[6] = "June";
$mesnome[7] = "July";
$mesnome[8] = "August";
$mesnome[9] = "September";
$mesnome[10] = "October";
$mesnome[11] = "November";
$mesnome[12] = "December";

$ano = date('Y');
$mes = date('n');
$dia = date('d');
$diasem = date('w');
$hora=getdate();

//hora certa para imprimir e enviar para o postgres
$horacerta=($hora['hours'].':'.$hora['minutes']);

// para inserir a data no postgres
$data_server = "$ano-$mes-$dia";

// data pronta para ser imprimida
$datacerta = $diasemana[$diasem].', '.$dia.' de '.$mesnome[$mes].' de '.$ano.' - Hora: '.$horacerta;
//echo "<center>$datacerta</center>";
?>
