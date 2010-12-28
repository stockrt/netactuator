	

<form action="graficos.php" method="get"> 
<b>Buscar host:</b> 
<input type="text" name="criterio" size="22" maxlength="150"> 
<input type="submit" value="Buscar"> 
</form> 
<? 


include "include/estilo.css";
include "include/connect.php";
include "include/functionLayout.php";
include "include/functions.php";
//include "include/refresh.php";

// Precisa ter o time aqui nesta pagina tambem, ver como fazer dai...
$sql_time = "SELECT flow_capture_time_min FROM config_netactuator";
$res_time = mysql_query($sql_time);
$time = mysql_result($res_time, 0);
$time = $time*60;


//inicio o critério e recebo qualquer cadeia que se deseje procurar 
$criterio = ""; 
if ($_GET["criterio"]!=""){ 
    $txt_criterio = $_GET["criterio"]; 
    $criterio = " WHERE host LIKE '%" . $txt_criterio . "%'"; 
} 

//Limito a busca 
$TAMANHO_PAGINA = 20; 
//examino a página a mostrar e o inicio do registro a mostrar 
$pagina = $_GET["pagina"]; 
if (!$pagina) { 
    $inicio = 0; 
    $pagina=1; 
} 
else { 
    $inicio = ($pagina - 1) * $TAMANHO_PAGINA; 
} 
//vejo o número total de campos que há na tabela com essa busca 
//$ssql = "SELECT host FROM storage_pattern_def " . $criterio; 
$ssql = "SELECT DISTINCT host FROM storage_mass " . $criterio; 
$rs = mysql_query($ssql,$connect); 
$num_total_registos = mysql_num_rows($rs); 

//calculo o total de páginas 
$total_paginas = ceil($num_total_registos / $TAMANHO_PAGINA); 

//ponho o número de registros total, o tamanho de página e a página que se mostra 
echo "Número de registros encontrados: " . $num_total_registos . " ($pagina - $total_paginas)<br><br>"; 
//echo "Mostram-se páginas de " . $TAMANHO_PAGINA . " registros cada uma<br>"; 
//echo "A mostrar a página " . $pagina . " de " . $total_paginas . "<p>"; 

//construo a sentença SQL 
//$ssql = "SELECT host FROM storage_pattern_def " . $criterio . " LIMIT " . $inicio . "," . $TAMANHO_PAGINA; 
$ssql = "SELECT DISTINCT host FROM storage_mass " . $criterio . " LIMIT " . $inicio . "," . $TAMANHO_PAGINA; 
$rs = mysql_query($ssql);
$max = mysql_num_rows($rs);

for($i=0;$i < $max; $i++) {
	list($dados) = mysql_fetch_row($rs);
	//echo "<a href='graphics.php?host=$dados' traget='main'>$dados</a> <br>";
// coloquei (rogerio) para imprimir o valor do path e descobri que time era o problema dos graficos nao aparecerem mais (devido ao refresh.php ter mudado)
//	echo "graph/".$dados."_".$time."_traf_day.png";
        if(file_exists("graph/".$dados."_".$time."_traf_day.png")){
    		print "<b>".$dados."&nbsp;&nbsp;&nbsp;&nbsp;<a href='graphics.php?host=".$dados."&type=traf' target='main'>traf</a>
    	 &nbsp;&nbsp;<a href='graphics.php?host=".$dados."&type=base' target='main'>base</a><br></b>";
        }
        if(!file_exists("graph/".$dados."_".$time."_traf_day.png"))
    			print $dados." - Gráfico não disponível<br>";

}
echo("<br><br>");
//fechamos o conjunto de resultado e a conexão com a base de dados 
mysql_free_result($rs); 
mysql_close($connect); 

//mostro os diferentes índices das páginas, se é que há várias páginas 
if ($total_paginas> 1){ 
    for ($i=1;$i<=$total_paginas;$i++){ 
       if ($pagina == $i) 
          //se mostro o índice da página atual, não coloco link 
          echo "| <b> $pagina </b> "; 
       else 
          //se o índice não corresponde com a página mostrada atualmente, coloco o link para ir a essa página 
          echo "| <a href='graficos.php?pagina=" . $i . "&criterio=" . $txt_criterio . "'> " . $i . "</a> "; 
    } 
	echo " |";
} 

?> 


<br />
<br />
<a href="menu.php" target="menu"><center><b><- Retornar ao menu</b></center></a>


