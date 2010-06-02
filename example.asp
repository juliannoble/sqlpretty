<%@ Language="VBScript" %>
<!-- #include virtual="/sqlpretty/sqlpretty.inc" -->
<html>
<head>
    <link rel="stylesheet" type="text/css" href="/sqlpretty/sqlpretty.css"> 
</head>
<body>
<%
dim sql
sql = "select o.*, a.*, c.* from ((addresses as a inner join orders as o on a.id=o.deliveryid) inner join (select users.id from users,orders where orders.userid=user.id group by users.id having count(orders.orderid) >= 1) as u on o.userid=u.id) Where o.type='x' order by o.id;"
sql = sql & "ALTER TABLE `product` ADD COLUMN `c3` VARCHAR(255) AFTER `c2`, ADD COLUMN `abc` VARCHAR(255) AFTER `xyz`;"

sql_pretty_default_hide()  'we are using the javascript - so we don't want the output visible by default (avoid annoying reflow on load) .
response.write sql_pretty(sql)
%>
    <script type="text/javascript" src="http://code.jquery.com/jquery-1.4.2.min.js"></script>
    <script type="text/javascript">jQuery.noConflict();</script>
    <script type="text/javascript" src="/sqlpretty/sqlpretty.js"></script>
</body>
</html>

