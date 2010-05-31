<!-- #include file="sqlpretty.inc" -->
<%
dim fso,ts
set fso = server.CreateObject("Scripting.FilesystemObject")
Set ts = fso.OpenTextFile(server.mappath("sqlpretty.css"))
css_data = ts.ReadAll
ts.Close

response.write "" & vbcrlf & _
"<style type=""text/css"">" & vbcrlf & _
css_data & vbcrlf & _
"</style>" & vbcrlf
%>
