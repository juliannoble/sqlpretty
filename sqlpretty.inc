<%
'JMN  - Released under BSD license. No warranty.

'*****
' NOTE: the sql_pretty function requires some extra javascript to enable hide/display (jQuery)
' use something like the following:
'<script type="text/javascript" src="/lib/jquery.js"></script>
'<script type="text/javascript">jQuery.noConflict();</script>
'<script type="text/javascript">
'jQuery(document).ready(function() {
'    jQuery('.sqlpretty-result').hide();
'    jQuery('.sqlpretty-hide-show').each(function(idx) {
'        var lnkname = jQuery(this).html()
'        jQuery(this).html('Show ' + lnkname);
'        jQuery(this).click(function() {
'            var debugdiv = jQuery(this).parent().parent();
'            var resdiv = debugdiv.find('.sqlpretty-result');
'            var lnk = debugdiv.find('.sqlpretty-hide-show');
'            if (resdiv.is(':visible')) {
'                lnk.html('Show ' + lnkname);
'                resdiv.hide();
'            } else {
'                lnk.html('Hide ' + lnkname);
'                resdiv.show();
'            }
'            return false;
'        });
'        jQuery(this).parent().css('visibility','visible');
'    });
'});
'</script>
'*****

dim SQL_PRETTY_LINKNAME
SQL_PRETTY_LINKNAME = "SQL" 'default name - becomes "Show / Hide SQL"
dim SQL_PRETTY_DEFAULT_HIDDEN
SQL_PRETTY_DEFAULT_HIDDEN = false  'so that sql_pretty is useful on javascriptless pages. Call sql_pretty_default_hide() to stop reflow-flicker if using javascript.


'-----------------------------------------------
'API 

function sql_pretty_default_hide() 
    SQL_PRETTY_DEFAULT_HIDDEN = true
end function
function sql_pretty_default_show() 
    SQL_PRETTY_DEFAULT_HIDDEN = false
end function


function sql_pretty_named(sqlstr,link_name)
    SQL_PRETTY_LINKNAME = link_name
    sql_pretty_named = sql_pretty(sqlstr)
end function

function sql_pretty(byval sqlstr)
    dim i
    dim k
    set k = server.createobject("Scripting.Dictionary")
    
    k.add "(", "bracket"
    k.add ")", "bracket"
    k.add "select", "primary"
    k.add "from", "primary"
    k.add "where", "primary"
    k.add "like", "primary"
    k.add "group", "primary"
    k.add "having", "primary"
    k.add "order", "primary"
    k.add "by", "primary"
    k.add "in", "primary"
    k.add "set", "primary"
    k.add "values", "primary"
    
    k.add "insert", "write"
    k.add "update", "write"
    k.add "delete", "write"
    
    k.add "into", "secondary"  
    
    k.add "count", "function"
    k.add "avg", "function"
    k.add "max", "function"
    k.add "min", "function"
    k.add "sum", "function"
    k.add "iif", "function"
    
    'data def related
    k.add "alter", "write"
    k.add "add", "write"
    k.add "table", "object"
    k.add "column", "object"
    k.add "index", "object"
    k.add "after", "secondary"
    k.add "before", "secondary"

    k.add "varchar", "function"
    k.add "int", "function"
    
    k.add "default", "secondary"
    k.add "null", "secondary"
    
    k.add "as", "secondary"
    k.add "on", "secondary"
    k.add "top", "secondary"
    k.add "distinct", "secondary"
    k.add "unique", "secondary"
    
    k.add "or", "logic"
    k.add "and", "logic"
    k.add "between", "logic"
    
    k.add "inner", "join"
    k.add "outer", "join"
    k.add "join", "join"
    k.add "left", "join"
    k.add "right", "join"
    k.add "full", "join"
    k.add "cross", "join"  
    
    sqlstr = replace(sqlstr,vbtab,"    ") 'normalize
    
    dim wordterminals, result, word, prev_word_func, func_active, join_keyword_sequence
    dim opendiv, closediv, first_in_div
    dim open_bracket_newline, bracket
    dim ch,breakdist
    
    wordterminals = "| |,|" & chr(13) &"|" & chr(10) & "|(|)|;|"
    result = ""
    word = ""
    prev_word_func = false
    func_active = false
    join_keyword_sequence = false 'whether we are in a sequence of join keywords.
    opendiv = ""
    closediv = ""
    first_in_div = false
    for i = 1 to len(sqlstr)
        ch=mid(sqlstr,i,1)
            if instr(wordterminals,"|" & ch & "|") then
                if k.exists(lcase(word)) then
                    if (k.item(lcase(word)) <> "function") then
                        prev_word_func = false
                    else
                        prev_word_func = true
                    end if
                end if
                
                'default assumption - formatted ch is just the ch - may be overridden for items such as brackets.
                formatted_ch = ch
                
                if k.item(ch) = "bracket" then
                    opendiv = ""
                    closediv = ""
                    bracket = ch
                    open_bracket_newline = ""
                    'brackets are self terminating words
                    if bracket = "(" then
                        if first_in_div = true then
                            first_in_div = false
                        else
                            open_bracket_newline = vbcrlf
                        end if
                    
                        if prev_word_func = true then
                            func_active = true
                            prev_word_func = false
                        end if
                        if func_active = false then
                            opendiv = "<div class=""sqlindent"">" 
                            first_in_div = true
                        end if
                    end if
                    if bracket=")" then
                        if func_active = false then
                            closediv = "</div>"
                            first_in_div = false 'Most likely already set false by previous elements - (assert: first_in_div false after close bracket)
                        end if
                    end if


                    if  func_active = true then
                        'no newline 
                        formatted_ch =  "<span class=""sqlbracket"">" & ch & "</span>"
                    else
                        if bracket="(" then
                            formatted_ch = open_bracket_newline & "<span class=""sqlbracket"">" & ch & "</span>"
                        else
                            formatted_ch = "<span class=""sqlbracket"">" & ch & "</span>"
                        end if
                    end if
                    
                    formatted_ch =  closediv & formatted_ch & opendiv 'Strange but true!  Only one of closediv & opendiv have a value at one time. They are deliberately in reverse order so that the ch is outside the div.

                    if bracket = ")" then
                        func_active = false
                    end if

                    
                    
                end if
                
                if (ch = ";") then
                    formatted_ch = ";" & vbcrlf
                end if
                
                
                breakdist = 6  'just enough to avoid adding extra linebreaks if raw sql already happens to be formatted with carriage returns.
                if k.exists(lcase(word)) then
                    if first_in_div = false then
                        'hack for 'by' in "order by" and "group by"
                        if ((k.item(lcase(word)) = "primary") and lcase(word) <> "by" ) or k.item(lcase(word)) = "write" or ((k.item(lcase(word)) = "join") and join_keyword_sequence = false ) then
                            'see if there is a line break up to breakdist chars before the keyword
                            start_test = i - breakdist
                            if start_test < 1 then
                                start_test = 1
                            end if
                            test = mid(sqlstr,start_test,breakdist)
                            'response.write lcase(word) & " XXX " & test & "<br><br>" &  vbcrlf & vbcrlf
                            if instr(test,vbcrlf) <= 0 then
                                'didn't find a break just before this keyword - let's insert one
                                'sqlstr = mid(sqlstr,1,posn-1) & vbcrlf & mid(sqlstr,posn)
                                br = vbcrlf 
                            else
                                br = ""
                            end if
                        else
                            br = ""
                        end if
                    else
                        'don't add a linebreak for 1st element in a div - it already appears to be on a new line due to the div.
                        first_in_div = false
                        br = ""
                    end if
                    if k.item(lcase(word)) = "join" then
                        join_keyword_sequence = true
                    else 
                        join_keyword_sequence = false
                    end if

                    result = result & br & "<span class=""sqlword " & k.item(lcase(word)) & """>" & word & "</span>" & formatted_ch
                else
                    join_keyword_sequence = false
                    result = result & word & formatted_ch
                end if
                word = ""
            else
                word = word & ch
            end if
    next
    result = result & word
    
    set k = nothing
    
    result = replace(result,vbcrlf,"<br/>" & vbcrlf)
    
    if SQL_PRETTY_DEFAULT_HIDDEN then
        'use visibility hidden because we want to reserve space for the link in the html flow.
        handle_display_style = "style=""visibility: hidden;"""
        
        'use display 'none' so we don't reserve any space for the result block in the layout.
        result_display_style = "style=""display: none;"""
    else
        'Presumably we are not using javascript - default to display
        handle_display_style = ""  
        result_display_style = ""
    end if
    
    sql_pretty = vbcrlf & _
    "<div id=""sqlpretty-" & replace(SQL_PRETTY_LINKNAME," ","") & """ class=""sqlpretty"">" & vbcrlf & _
    "    <div class=""sqlpretty-handle"" " & handle_display_style & ">" & vbcrlf & _
    "        <a href=""#"" class=""sqlpretty-hide-show"">" & SQL_PRETTY_LINKNAME & "</a>" & vbcrlf & _
    "    </div>" & vbcrlf & _
    "    <div class=""sqlpretty-result"" " & result_display_style & ">" & vbcrlf & _
    "        <span class=""sqlpretty-css-warning"">(add a link to /sqlpretty/sqlpretty.css to see coloured output)</span>" & vbcrlf & _
    "        <div class=""sqlpretty-formatted"">" & vbcrlf & _
    result & vbcrlf & _
    "        </div>" & vbcrlf & _ 
    "        <div class=""sqlpretty-raw"">" & vbcrlf & _
    "            <pre>" & _
    sqlstr & _
    "            </pre>" & vbcrlf & _
    "        </div>" & vbcrlf & _ 
    "    </div>" & vbcrlf & _
    "</div>" & vbcrlf
end function
'-----------------------------------------------
%>
