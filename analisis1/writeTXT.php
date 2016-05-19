<?php
    $name = $_POST["fileName"];
    $text = $_POST["text"];
		
	$file = fopen("problemas/".$name, "w");

	fwrite($file, $text . PHP_EOL);

	fclose($file);
?>