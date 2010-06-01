<html>
<head>
</head>
<body>


<%
 'A <style> block will be emitted at the point where sqlpretty_css_internal.inc is included so strictly speaking - the include should be done within the <head>
 ' In practice - placing the style in the body works, and as this is intended for debug purposes - it's handy to just be able to throw in the include and call the sql_pretty function.
%>
<!-- #include virtual="/sqlpretty/sqlpretty_css_internal.inc" -->

<hr>
<%
dim sql
sql = "select o.*, a.*, c.* from ((addresses as a inner join orders as o on a.id=o.deliveryid) inner join (select users.id from users,orders where orders.userid=user.id group by users.id having count(orders.orderid) >= 1) as u on o.userid=u.id) Where o.type='x' order by o.id;"
sql = sql & "ALTER TABLE `product` ADD COLUMN `c3` VARCHAR(255) AFTER `c2`, ADD COLUMN `abc` VARCHAR(255) AFTER `xyz`;"
response.write sql_pretty(sql)
%>
<hr>

</body>
</html>

