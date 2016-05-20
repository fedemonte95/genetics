<?php
set_time_limit(0);

//var_dump($_GET);


$GA_POPSIZE      = array_key_exists ("poblacion", $_GET)? $_GET["poblacion"] : 0;
$GA_MAXITER      = array_key_exists ("generaciones", $_GET)? $_GET["generaciones"] : 0;
$GA_ELITRATE     = array_key_exists ("poblacion", $_GET)? $_GET["poblacion"] : 0;
$GA_MUTATIONRATE = array_key_exists ("mutacion", $_GET)? $_GET["mutacion"] : 0;

define("GA_POPSIZE", $GA_POPSIZE);
define("GA_MAXITER", $GA_MAXITER); //16384);	
define("GA_ELITRATE", 0.10);
define("GA_MUTATIONRATE", 0.1);
define("GA_MUTATION", getrandmax() * GA_MUTATIONRATE);

abstract class Problema{
	function readProblema(){
		$txt = array();
		$_POST["noJSON"] = 1;
		$_POST["fileName"] = array_key_exists ("archivoDatos", $_GET)? $_GET["archivoDatos"] : "";
		include("readTXT.php");
		$this->name = trim($txt[0]);
		list($this->size, $this->M, $this->K) = explode(" ", trim($txt[1]));
		
		return $txt;
	} // Lee un problema de un archivo
	function geneSize(){
		return $this->size;
	} // tama~no del gen
	function getfitness(){}// calculo del fitness
	function name(){
		return $this->name;
	}
}


class Estructura {
	function setstr($str) {
		$this->str .= $str;
	}
	function generacion() {
		return $this->str;
	}
	function setfitness($ints) {
		$this->ints = $ints;
	}
	function getfitness() {
		return $this->ints;
	}
	function erasestr() {
		$this->str = "";
	}
	function erasefitness() {
		$this->ints = "";
	}
}

class Evolutiva extends Problema {
	
	function __construct($N, $M, $K) {
		$this->crearGrafo($this->readProblema());
		echo $this->name();
		//echo $this->geneSize();
		
		$this->contador = 0;
		$this->iniciar_poblacion();
		
		for ($i = 0; $i < GA_MAXITER; $i++) {
			
			$this->fitness();
			
			$this->ordenar_por_seleccion();
			
			if ($i+1 == GA_MAXITER) {
				echo "<br/><strong>MEJOR RESPUESTA: </strong><br/>";
				$this->imprimir_mejor();
				echo "<hr/><br/>";
				break;
			}
			
			$this->imprimir_mejor();
			
			$this->combinar();
			$this->reiniciar();
		}
	}
	
	function crearGrafo($datos) {
		$this->grafo = array();
		$this->N = array();
		for($i = 0; $i < $this->geneSize(); $i++){
			$this->N[] = $i;
		}
		$countArcos = 0;
		foreach ($datos as $num_linea => $linea) {
			if ($num_linea >= 2 && $countArcos < $this->M){
//				$countArcos++;
				list($nodoi, $nodoj) = explode(" ", trim($linea));
				if (! (array_key_exists("#".$nodoi."#".$nodoj."#", $this->grafo) || array_key_exists("#".$nodoj."#".$nodoi."#", $this->grafo)) ){
					$this->grafo["#".$nodoi."#".$nodoj."#"] = 0;
				}
			}
		}
		
//		echo "<pre>";
//		echo "nodos = ".count($this->N);
//		echo "arcos = ".count($this->grafo);
//		var_dump($this->grafo);
//		echo "</pre>";
	}	
	
	function clearGrafo() {		
		foreach ($this->grafo as $key => $value) {
			$this->grafo[$key] = 0;
		}
	}
	
	function iniciar_poblacion() {
		
		$tsize = $this->geneSize();
		
		for ($i = 0; $i < GA_POPSIZE; $i++) {			
			$citizen = new Estructura();
			$bifer   = new Estructura();
			
			$citizen->setfitness(0);
			$citizen->erasestr();
			
			$bifer->setfitness(0);
			$bifer->erasestr();
			$poblacion="";
			
			while (strlen($poblacion) < $this->geneSize()){
				$poblacion .= rand(0, 1);
			}
			
			$citizen->setstr($poblacion);
			
//			echo strlen($citizen->generacion())." #".$i."<br>";
			
			$this->population[] = $citizen;
			$this->buffer[]     = $bifer;
		}
		
//		echo "<pre>";
//		var_dump($this->population);
//		echo "</pre>";
//		
//		echo "<pre>";
//		var_dump($this->buffer);
//		echo "</pre>";
//		
	}
	function fitness() {
		
		$tsize  = $this->geneSize();
		/*
http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.65.9686&rep=rep1&type=pdf #4
		*/
		for ($i = 0; $i < GA_POPSIZE; $i++) {
			$fitness = 0;
			$this->clearGrafo();
//			echo "fitnes: ".$fitness."<pre>";
//			var_dump($this->grafo);
//			echo "</pre>";
			$lcadena = $this->population[$i]->generacion();
			for ($j = 0; $j < $tsize; $j++) {
				if($lcadena[$j]){
					foreach($this->grafo as $key => $value){
						if (strpos($key, "#".$this->N[$j]."#") !== false) {
							$fitness++;
							$this->grafo[$key] = 1;
						}
					}
				} else {
					$fitness--;
				}
			}
			
			//$fitness = $unos * $ceros;
			
			foreach($this->grafo as $key => $value){
				if(!$value){
					$fitness += $tsize;
				}
			}
			
//			if ($fitness == 62){
//			echo "fitnes: ".$fitness."<pre>";
//			var_dump($this->grafo);
//			echo "</pre>";}
			
			$this->population[$i]->setfitness($fitness);
		}
		
	}
	function ordenar_por_seleccion() {
		
		for ($i = 0; $i < count($this->population); $i++) {
			
			$arraygeneral["f"][$i] = $this->population[$i]->getfitness();
			$arraygeneral["g"][$i] = $this->population[$i]->generacion();
			
		}
		array_multisort($arraygeneral["f"], //SORT_DESC, 
						$arraygeneral["g"]//, SORT_DESC
					   );
		
		for ($i = 0; $i < count($this->population); $i++) {
			
			$this->population[$i]->erasestr();
			$this->population[$i]->erasefitness();
			
			$this->population[$i]->setfitness($arraygeneral["f"][$i]);
			$this->population[$i]->setstr($arraygeneral["g"][$i]);
		}
		
	}
	function elitismo($esize) {
		
		for ($i = 0; $i < $esize; $i++) {
			
			$this->buffer[$i]->erasestr();
			$this->buffer[$i]->erasefitness();
			
			$this->buffer[$i]->setstr($this->population[$i]->generacion());
			$this->buffer[$i]->setfitness($this->population[$i]->getfitness());
			
		}
	}
	function mutacion($memberid) {
		
		
		$tsize = (int) ($this->geneSize());
		$ipos  = (int) (rand() % $tsize);
		
		$lcadena       = $this->buffer[$memberid]->generacion();
		
		for ($i = 0; $i < strlen($lcadena); $i++) {
			if ($i == $ipos) {
				$lcadena[$i] == !$lcadena[$i];
			}
		}
		
		$this->buffer[$memberid]->erasestr();
		$this->buffer[$memberid]->setstr($lcadena);
		
	}
	function combinar() {
		$esize = (int) (GA_POPSIZE * GA_ELITRATE); //204
		$tsize = $this->geneSize();
		$this->elitismo($esize);
		
		for ($i = $esize; $i < GA_POPSIZE; $i++) {
			$i1   = rand() % (GA_POPSIZE / 2);
			$i2   = rand() % (GA_POPSIZE / 2);
			$spos = rand() % $tsize;
			
			$this->buffer[$i]->erasestr();
			
			$this->buffer[$i]->setstr(
				substr(
					$this->population[$i1]->generacion(), 0, $spos
				) .
				substr(
					$this->population[$i2]->generacion(), $spos, $esize - $spos
				)
			);
			
			if (rand() < GA_MUTATION) {
				$this->mutacion($i);
			}
		}
		
	}
	function imprimir_mejor() {
		
		echo $this->contador++ ;
		echo "	Generacion: " . $this->population[0]->generacion();
		echo "	Fitness: " . $this->population[0]->getfitness();
		echo "<br/>";
		flush();
		ob_flush();
	}
	
	function reiniciar() {
		$temp             = $this->population;
		$this->population = $this->buffer;
		$this->buffer     = $temp;
	}
	function make_seed() {
		list($usec, $sec) = explode(' ', microtime());
		return (float) $sec + ((float) $usec * 100000);
	}
}

//$objeto = new Evolutiva(
//	array(1,2,3,4,5,6), 
//	array(array(2,3), array(1,3,4,5,6), array(1,2), array(2), array(2), array(2) ), 
//	2
//);

$objeto = new Evolutiva(
	array(0,1,2,3,4,5,6,7,8,9), 
	array(array(1,4,5), 
		  array(0,2,6), 
		  array(3,1,7), 
		  array(4,2,8), 
		  array(0,3,9), 
		  array(0,7,8), 
		  array(1,9,8), 
		  array(5,9,2), 
		  array(6,5,3), 
		  array(6,7,4), ), 
	2
);
?>