<?php
    $name = $_POST["fileName"];
		
	$file = fopen("problemas/".$name, "r");

	while(!feof($file)){
		$txt[] =  
			trim(
				preg_replace(
					'/\s+/', 
					' ', 
					fgets($file)
				)
			);
	}
	fclose($file);

	if (!array_key_exists ("noJSON", $_POST)){
		$response = array(
			"success" => 1,
			"txt" => $txt,
		); 

		header('Content-type: application/json; charset=utf-8');

		echo json_encode($response, JSON_FORCE_OBJECT);
	}

?>