<?php
include "include/estilo.css";
include "include/connect.php";
include "include/functionLayout.php";
include "include/functions.php";
//include "include/refresh.php";

if (isset($tb)) {
    $fields = mysql_list_fields("netactuator", $tb, $connect);

    for ($i = 0; $i < mysql_num_fields($fields); $i++) {
        $field[$i] = mysql_field_name($fields, $i);
    }

    if ($action=='insert')
        formInsertDados($tb, $field);
    elseif ($action=='alter')
        formUpdateItem($tb, $campo, $dado, $i, $id, $data, $erro, $action);
    elseif($action=='delete')
        deleteDados($tb, $dado);

    $formTop = formTop($tb, $field);

    // verifica tabela TOP_USERS
    if($tb=='net_topusers') {
        if($toptype) {
            $formTopUsersFields = formTopUsersFields($tb, $field, $i, $action);
            $selectTopUsers = selectTopUsers($toptype);
        } else {
            $formTopUsersFields = formTopUsersFields($tb, $field, $i, $action);
            $getDados = getDados($tb, $field, $i);
        }
    } else {
        views($tb, $view_insert, $view_delete, $view_id, $view_alter, $view_table);
        $formFields = formFields($tb, $field, $i, $action, $view_insert, $view_alter, $view_id);
        $getDados = getDados($tb, $field, $j, $view_delete, $view_alter, $view_id, $dados);
        //getDados($tb, $field, $i);
    }

    print "</tr></table>";
} else {
    echo "<BR><BR><BR> <B> <CENTER> WELCOME TO netactuator. </CENTER> </B>";
}

if($action=='list')
    print $erro;
?>
