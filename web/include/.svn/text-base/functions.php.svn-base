<?php
function mysql_table_exists($row)
{
//     $exists = mysql_query("SELECT 1 FROM `$table` LIMIT 0");
     $exists = mysql_query("SELECT * FROM config_tables WHERE table_name='".$row[0]."' AND table_view='1'");
     if ($exists) return true;
     return false;
}

function checkViewTables($row) {
	$sql = "SELECT * FROM config_tables WHERE table_name='".$row[0]."' AND table_view='1'";
	$res = mysql_query($sql);
	if (mysql_num_rows($res)>'0')
		return true;
		//print "1";
	else {
		$sql = "SELECT * FROM config_tables WHERE table_name='".$row[0]."'";
		$res = mysql_query($sql);

		if (mysql_num_rows($res)=='0')
			return true;
		else 
			return false;
	}
	
}


function getDados($tb, $field, $j, $view_delete, $view_alter, $view_id, $dados) {

	global $tb, $field, $j, $view_delete, $view_alter, $view_id, $dados;
	$sql = "SELECT * FROM ".$tb."";
	$res = mysql_query($sql);

	if($tb != 'config_tables'){
	
		while ($dados = mysql_fetch_array($res)) {
			print"<tr>";
			for($j=0; $j < count($dados); $j++) {
				if($field[$j] == 'id') {
					if($view_id == '1') {
						print "<td> ".$dados[$j]." </td>";
					}
				} else {
					if($view_alter == '1') {
						print "<td><div align='center'><a href='?tb=$tb&action=alter&campo=".$field[$j]."&dado=".$dados[$j]."&id=".$dados[0]."'>".$dados[$j]."</a></div></td>";	
					} else {
						if($tb == 'net_topusers' && $field[$j] == 'host') // host com link
							print "<td><div align='center'><a href=graficos.php?criterio=".$dados[$j]."> ".$dados[$j]." </a></div></td>";
						else
							print "<td><div align='center'> ".$dados[$j]." </div></td>";
					}
				}
			}
			if($view_delete=='1')
				print "<td width='20'><div align='left'><a href='?tb=$tb&action=delete&id=".$dados[0]."'>deletar</a></div></td>";
			print"</tr>";
		}
		
	} else {
			print "<tr><td colspan=".count($field)." bgcolor='#E9E9E9'><a href='main.php?tb=$tb&action=insert' target='main'>Inserir novo</a></td></tr>";

		while ($dados = mysql_fetch_array($res)) {
			print"<tr>";
			for($j=0; $j < count($dados); $j++) {
				if($field[$j] != 'id')
					print "<td><div align='center'><a href='?tb=$tb&action=alter&campo=".$field[$j]."&dado=".$dados[$j]."&id=".$dados[0]."'>".$dados[$j]."</a></div></td>";	
			}
			print "<td width='20'><div align='center'><a href='?tb=$tb&action=delete&id=".$dados[0]."'>deletar</a></div></td>";
			print"</tr>";
		}

	}
}


function updateDados($tb, $campo, $dado, $j, $id, $data, $connect, $erro) {
	if(isset($_POST['data']) && ($_POST['data'] != '')) {
		$sql = "UPDATE $tb SET $campo='".$_POST['data']."' WHERE id='$id'";
		$update = mysql_query($sql);
		echo "<b>table</b> $tb <br> <b>campo</b> $campo <br> <b> dado </b> $dado <br> <b>data:</b> $data <br><b>id $id</b> <br> $sql";
		$action='';
		$erro = "<b> $campo </b> alterado com sucesso!";
		header('location:main.php?tb='.$tb.'&action=list');
	}
}

function insertDados($tb, $field) {
		//$exec='1';
		for($i=0; $i < count($field); $i++) {
			if (!empty($_POST[$field[$i]])) {
				$exec='1';
				if($field[$i] != 'id'){
					if($i > 0) {
						if($i==1) {
							$SQL_fields = $field[$i];
							$SQL_dados = "'".$_POST[$field[$i]]."'";
						} else {
							$SQL_fields = "$SQL_fields, ".$field[$i]."";
							$SQL_dados = "$SQL_dados, '".$_POST[$field[$i]]."'";
						}
					} else
						$sql2 = $field[$i];
				}
			} elseif (empty($_POST[$field[$i]])) {
				$exec = '0';
			}
		}
		if($exec=='1'){
			$sql = "INSERT INTO $tb ($SQL_fields) VALUES($SQL_dados);";
			$update = mysql_query($sql);
			header('location:main.php?tb='.$tb.'');
		} elseif($exec=='0') {
			print "<font color='red'><b>Todos os campos devem ser preenchidos.</b></font>";
		}
}

function deleteDados($tb, $dado) {
	if($_GET['id']) {
		$sql = "DELETE FROM $tb WHERE id='".$_GET['id']."';";
		$query = mysql_query($sql);
	} else
		echo("Erro. Valor não setado!");
}

function mysql_fetch_all($result) {
   while($row=mysql_fetch_array($result)) {
       $return[] = $row;
   }
   return $return;
}

function getHostGraphics($dados) {
	$sql = "SELECT host FROM storage_pattern_def";
	$res = mysql_query($sql);
	$max = mysql_num_rows($res);
	
	for($i=0;$i < $max; $i++) {
		list($dados) = mysql_fetch_row($res);
		if(file_exists("graph/".$dados."_".$time."_day.png"))
			print "<center><b><a href='graphics.php?host=".$dados."'>".$dados."</a><br></b>";
		if(!file_exists("graph/".$dados."_".$time."_day.png"))
				print "<b>".$dados."</b> (Arquivo não encontrado) <br>";
	}
}

function checkConfigTables($row) {
	global $description;
	$sql = "SELECT table_name, delete_view, id_view, insert_view, description FROM config_tables WHERE table_name = '".$row[0]."';";
	$res=mysql_query($sql);
	$max=mysql_num_rows($res);

	if($max=='0')
		$description = $row[0];
	elseif($res){
		for($j=0; $j<$max; $j++) {
			list($tb_name, $delelte_view, $id_view, $insert_view, $description) = mysql_fetch_row($res);
		}
	} 
}

function views($tb, $view_insert, $view_delete, $view_id, $view_alter) {
	global $view_insert, $view_delete, $view_id, $view_alter, $view_table;
	$sql = "SELECT id_view, insert_view, delete_view, table_view, alter_view FROM config_tables WHERE table_name='".$tb."' AND table_name != 'config_tables'";
	$res = mysql_query($sql);

	while ($views = mysql_fetch_array($res)) {
		for($j=0; $j < 5; $j++) {
			$id_insert = $views[0];
			$view_insert = $views[1];
			$view_delete = $views[2];
			$view_table = $views[3];
			$view_alter = $views[4];
		}
	}
}



?>
