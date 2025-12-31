<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8" />
    <title>Hello, world!</title>
  </head>
  <body>
    <h1><%= "Hello, world! page1" %></h1>
    <div id="text">
    </div>
  </body>
</html>
<script type="text/javascript">
  let data = {
  <%
  for(int i=0;i<10;i++){
    if (i !=0)	out.println(",");
      out.println( "data" +i + ":" + i);
    }
    %>
  };

  temp = document.getElementById("text");
  temp.innerText = JSON.stringify(data);
</script>
