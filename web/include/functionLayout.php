<?php
function formTop($tb, $field) {
    print "
    <table width='700' border='0' cellspacing='2' cellpadding='2'>
      <tr>
        <td colspan='".count($field)."' bgcolor='#CCE6FF'><div align='center'><strong> [ $tb ] </strong></div></td>
      </tr>
    ";
}

function topTabelas() {
    print "
        <table width='80%' border='1' cellspacing='2' cellpadding='2' bordercolor='#FFFFFF'>
          <tr>
            <td colspan='10' bgcolor='#CCE6FF' bordercolor='#000000'><div align='center'><strong>Tables</strong></div></td>
          </tr>
          <tr>
    ";
}

function formTables($row, $description) {
    print"
        <tr>
            <td bordercolor='#000000'><strong>
                &nbsp;&nbsp; <a href=main.php?tb=".$row[0]." target='main'>$description</a>
            </strong></td>
        </tr>

    ";
}

function formFields($tb, $field, $i, $action, $view_insert, $view_alter, $view_id) {
    if($view_insert=='1') {
        print "<tr><td colspan=".count($field)." bgcolor='#E9E9E9'><a href='main.php?tb=$tb&action=insert' target='main'>Inserir novo</a></td></tr>";
    }
    print "<tr>";
    for($j=0; $j < count($field); $j++)
        if($field[$j] == 'id') {
            if($view_id == '1')
                print "<td bgcolor='#E9E9E9'><b><div align='center'>".$field[$j]."</div></b></td>";
        } else {
            if($j==(count($field)-1))
                print "<td colspan='2' bgcolor='#E9E9E9'><b><div align='center'>".$field[$j]."</div></b></td>";
            else
                print "<td bgcolor='#E9E9E9'><b><div align='center'>".$field[$j]."</div></b></td>";
        }
    print "</tr>";
}

function formTopUsersFields($tb, $field, $i, $action) {
    print "<tr>";
    for($j=0; $j < count($field); $j++)
        print "<td bgcolor='#E9E9E9'><b><center><a href='main.php?tb=$tb&toptype=$j' target='main'>".$field[$j]."</a></center></b></strong></td>";
    print "</tr>";
}

function selectTopUsers($toptype) {
    $sql = "SELECT * FROM net_topusers WHERE toptype='$toptype'";
    $res = mysql_query($sql);

    while ($dados = mysql_fetch_array($res)) {
        print"<tr>";
        for($j=0; $j < count($dados); $j++) {
            if($field[$j] == 'id')
                print "<td><div align='center'> ".$dados[$j]." </div></td>";
            else {
                // print "<td><div align='center'><a href='main.php?tb=$tb&action=alter&campo=".$field[$j]."&dado=".$dados[$j]."&id=".$dados[0]."'>".$dados[$j]."</a></div></td>";
                if($j == 0) // host com link
                    print "<td><div align='center'><a href=graficos.php?criterio=".$dados[$j]."> ".$dados[$j]." </a></div></td>";
                else
                    print "<td><div align='center'>".$dados[$j]."</div></td>";
            }
        }
        print"</tr>";
    }
}

function formGraphics() {
    print"
        <tr>
            <td bordercolor='#000000'><strong>
                &nbsp;&nbsp; <a href='graficos.php'>Graphs</a>
            </strong></td>
        </tr>
    ";
}

function formDados($tb, $dados, $i) {
    print "<tr>";
    for($j=0; $j < count($dados); $j++)
        print "<td> ".$dados[$j]." </td>";
    print "</tr>";
}

function formUpdateItem($tb, $campo, $dado, $i, $id, $data, $erro, $action) {
    $form1 = formTop($tb, $field);
    $form2 = formFields($tb, $field, $i, $action, $view_insert, $view_alter, $view_id);

    print "
        <td>
        <form id='form1' name='form1' method='post' action='".updateDados($tb, $campo, $dado, $j, $id, $data, $connect, $erro)."'>
              <label>
                <input type='text' name='data' value='$dado'>
                </label>
              <label>
              <input type='submit' name='Submit' value='Submit' />
              </label>
            </form>
      </td>
    ";
}

function formInsertDados($tb, $field) {
    $form1 = formTop($tb, $field);
    $form2 = formFields($tb, $field, $i, $action, $view_insert, $view_alter, $view_id);

    print "<tr> <form name='form1' method='post' action='".insertDados($tb, $field)."'> ";

    for($i=0; $i < count($field); $i++) {
        // if($field[$i] == 'id')
        //    print"<td> AUTO </td>";
        //else
        if($field[$i] != 'id')
            print "
                <td><div align='center'>
                  <label>
                    <input type='text' name='$field[$i]' size='15'>
                  </label>
                  </div>
                  </td>
            ";
    }

    print "
        </tr>
        <tr>
          <td colspan='".count(field)."'>
            <label>  <input type='submit' name='Submit' value='Submit' /> </label>
          </td>
        </form>
        </tr>
    ";
}

function poweredby() {
    print"<br>
        <p class='style1small'>
            netactuator by Rogério Carvalho Schneider <br>
            Interface Web by Lucas Schneider
        </p>
    ";
}
?>
