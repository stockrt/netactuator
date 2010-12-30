<?php
/*
 * SAL - SQL Abstraction Library
 * version 0.01
 * Gianugo Rabellino <nemorino@opera.it>
 *
 * - 28/07: MySQL Support - mSQL support
 * - 29/07: PostgreSQL Support
 * - 03/08: ODBC Support
 */

/*
 *
 * Set the variable $dbtype to any of the following
 * values: MySQL, mSQL, Postgres, ODBC before including
 * this library
 *
 */

/* $dbtype = "MySQL"; */ /* $dbtype = "mSQL"; */ /* $dbtype = "PostgreSQL"; */ /* $dbtype = "ODBC"; */
function SQL_connect($host, $user, $password, $db) { $password = base64_decode($password);
global $dbtype; switch ($dbtype) {
    case "MySQL":
        $conn=mysql_pconnect($host, $user, $password);
        mysql_select_db($db);
        return $conn;
    break;;

    case "mSQL":
         $conn=msql_pconnect($host);
         msql_select_db($db);
         return $conn;
    break;;

    case "PostgreSQL":
         $conn=pg_connect("host=$host port='5432' dbname=$db user=$user password=$password");
         return $conn;
    break;;

    case "ODBC":
         $conn=odbc_pconnect($db,$user,$password);
         return $conn;
    break;;

    default:
        $conn=mysql_pconnect($host, $user, $password);
        mysql_select_db($db);
        return $conn;
    break;;
    }
}

/*
 * SQL_query($host, $user, $password, $db)
 * executes an SQL statement, returns a result identifier
 */
function SQL_query($query, $id) { global $dbtype; switch ($dbtype) {
    case "MySQL":
        $res=mysql_query($query, $id);
        return $res;
    break;;

    case "mSQL":
        $res=msql_query($query, $id);
        return $res;
    break;;

    case "PostgreSQL":
        $res=pg_query($id,$query);
        return $res;
    break;;

    case "ODBC":
        $rid=odbc_prepare($id,$query);
        $res=odbc_execute($rid);
        return $res;
    break;;

    default:
        $res=mysql_query($query, $id);
        return $res;
    break;;
    }
}

/*
 * SQL_num_rows($host, $user, $password, $db)
 * given a result identifier, returns the number of affected rows
 */
function SQL_num_rows($res) { global $dbtype; switch ($dbtype) {
    case "MySQL":
        $rows=mysql_num_rows($res);
        return $rows;
    break;;

    case "mSQL":
        $rows=msql_num_rows($res);
        return $rows;
    break;;

    case "PostgreSQL":
        $rows=pg_num_rows($res);
        return $rows;
    break;;

    case "ODBC":
        $rows=odbc_num_rows($res);
        return $rows;
    break;;

    default:
        $rows=mysql_num_rows($res);
        return $rows;
    break;;
    }
}

/*
 * SQL_fetchrow($res,$row)
 * given a result identifier, returns an array with the resulting row
 * Needs also a row number for compatibility with PostgreSQL
 */
function SQL_fetch_row($res, $nr)  { global $dbtype;  switch ($dbtype) {
    case "MySQL":
        $row = array();
        $row = mysql_fetch_row($res);
        return $row;
    break;;

    case "mSQL":
        $row = array();
        $row = msql_fetch_row($res);
        return $row;
    break;;

    case "PostgreSQL":
        $row = array();
        $row = pg_fetch_row($res,$nr);
        return $row;
    break;;

    case "ODBC":
        $row = array();
        $cols = odbc_fetch_into($res, $nr, &$row);
        return $row;
    break;;

    default:
        $row = array();
        $row = mysql_fetch_row($res);
        return $row;
    break;;
    }
}

/*
 * SQL_fetch_array($res,$row)
 * given a result identifier, returns an associative array
 * with the resulting row using field names as keys.
 * Needs also a row number for compatibility with PostgreSQL.
 */
function SQL_fetch_array($res, $nr)  { global $dbtype;  switch ($dbtype) {
    case "MySQL":
        $row = array();
        $row = mysql_fetch_array($res);
        return $row;
    break;;

    case "mSQL":
        $row = array();
        $row = msql_fetch_array($res);
        return $row;
    break;;

    case "PostgreSQL":
        $row = array();
        $row = pg_fetch_array($res,$nr);
        return $row;
    break;;

/*
 * ODBC doesn't have a native _fetch_array(), so we have to
 * use a trick. Beware: this might cause HUGE loads!
 */
    case "ODBC":
        $row = array();
        $result = array();
        $result = odbc_fetch_row($res, $nr);
        $nf = count($result)+2; /* Field numbering starts at 1 */
        for($count=1; $count < $nf; $count++) {
            $field_name = odbc_field_name($res, $count);
            $field_value = odbc_result($res, $field_name);
            $row[$field_name] = $field_value;
            }
        return $row;
        break;;
    }
}
