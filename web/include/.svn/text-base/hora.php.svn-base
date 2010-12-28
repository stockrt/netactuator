<?php 
$diasemana[0] = "Domingo"; 
$diasemana[1] = "Segunda-feira"; 
$diasemana[2] = "Terça-feira"; 
$diasemana[3] = "Quarta-feira"; 
$diasemana[4] = "Quinta-feira"; 
$diasemana[5] = "Sexta-feira"; 
$diasemana[6] = "Sábado"; 

$mesnome[1] = "janeiro"; 
$mesnome[2] = "fevereiro"; 
$mesnome[3] = "março"; 
$mesnome[4] = "abril"; 
$mesnome[5] = "maio"; 
$mesnome[6] = "junho"; 
$mesnome[7] = "julho"; 
$mesnome[8] = "agosto"; 
$mesnome[9] = "setembro"; 
$mesnome[10] = "outubro"; 
$mesnome[11] = "novembro"; 
$mesnome[12] = "dezembro"; 

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
