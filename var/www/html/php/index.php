<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
<body>
    <h1>php index page</h1>
    <?php
        echo '[아이디] : '.$_GET['id']. '<br/>';
        echo '[password] : '.$_GET['password'].' <br/>';
    ?>
</body>
