<?php

$GA_POPSIZE      = $_GET("poblacion");
$GA_MAXITER      = $_GET("generaciones");
$GA_ELITRATE     = $_GET("poblacion");
$GA_MUTATIONRATE = $_GET("mutacion");
$GA_TARGET       = $_GET("poblacion");

define("GA_POPSIZE", 2048);
define("GA_MAXITER", 100); //16384);	
define("GA_ELITRATE", 0.10);
define("GA_MUTATIONRATE", 0.1);
define("GA_MUTATION", getrandmax() * GA_MUTATIONRATE);
define("GA_TARGET", "DIFICIL");

class estructura {
	function setstr($str) {
		$this->str .= $str;
	}
	function getstr() {
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

class evolutiva {
	
	function __construct() {
		
		$this->contador = 0;
		$this->iniciar_poblacion();
		
		for ($i = 0; $i < GA_MAXITER; $i++) {
			
			$seed = $this->make_seed();
			mt_srand($seed);
			
			$this->calcular_seleccion();
			
			$this->ordenar_por_seleccion();
			
			if ($this->population[0]->getfitness() == 0) {
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
	
	function __constructVertexCover($N, $M, $K) {
		
		$this->contador = 0;
		$this->iniciar_poblacion();
		
		for ($i = 0; $i < GA_MAXITER; $i++) {
			
			$seed = $this->make_seed();
			mt_srand($seed);
			
			$this->calcular_seleccion();
			
			$this->ordenar_por_seleccion();
			
			$this->imprimir_mejor();
			
			if ($this->population[0]->getfitness() == 0) {
				break;
			}
			$this->combinar();
			$this->reiniciar();
		}
	}
	function iniciar_poblacion() {
		
		$tsize = strlen(GA_TARGET);
		
		for ($i = 0; $i < GA_POPSIZE; $i++) {
			
			$citizen = new estructura();
			$bifer   = new estructura();
			
			$citizen->setfitness(0);
			$citizen->erasestr();
			
			$bifer->setfitness(0);
			$bifer->erasestr();
			
			for ($j = 0; $j < $tsize; $j++) {
				$citizen->setstr(chr((rand() % 90) + 32));
			}
			
			$this->population[] = $citizen;
			$this->buffer[]     = $bifer;
		}
		
	}
	function calcular_seleccion() {
		
		$target = GA_TARGET;
		$tsize  = strlen(GA_TARGET);
		
		for ($i = 0; $i < GA_POPSIZE; $i++) {
			$fitness = 0;
			for ($j = 0; $j < $tsize; $j++) {
				$lcadena = $this->population[$i]->getstr();
				$fitness = $fitness + abs(ord($lcadena[$j]) - ord($target[$j]));
			}
			
			$this->population[$i]->setfitness($fitness);
		}
		
	}
	function ordenar_por_seleccion() {
		
		for ($i = 0; $i < count($this->population); $i++) {
			
			$arraygeneral["f"][$i] = $this->population[$i]->getfitness();
			$arraygeneral["g"][$i] = $this->population[$i]->getstr();
			
		}
		array_multisort($arraygeneral["f"], $arraygeneral["g"]);
		
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
			
			$this->buffer[$i]->setstr($this->population[$i]->getstr());
			$this->buffer[$i]->setfitness($this->population[$i]->getfitness());
			
		}
	}
	function mutacion($memberid) {
		
		
		$tsize = (int) (strlen(GA_TARGET));
		$ipos  = (int) (rand() % $tsize);
		$delta = (int) ((rand() % 90) + 32);
		
		$lcadena       = $this->buffer[$memberid]->getstr();
		$nuevocaracter = chr((ord($lcadena[$ipos]) + $delta) % 122);
		
		for ($i = 0; $i < strlen($lcadena); $i++) {
			if ($i == $ipos) {
				$lcadena[$i] = $nuevocaracter;
			}
		}
		
		$this->buffer[$memberid]->erasestr();
		$this->buffer[$memberid]->setstr($lcadena);
		
	}
	function combinar() {
		$esize = (int) (GA_POPSIZE * GA_ELITRATE); //204
		$tsize = strlen(GA_TARGET);
		$this->elitismo($esize);
		
		for ($i = $esize; $i < GA_POPSIZE; $i++) {
			$i1   = rand() % (GA_POPSIZE / 2);
			$i2   = rand() % (GA_POPSIZE / 2);
			$spos = rand() % $tsize;
			
			$this->buffer[$i]->erasestr();
			
			$this->buffer[$i]->setstr(substr($this->population[$i1]->getstr(), 0, $spos) . "" . substr($this->population[$i2]->getstr(), $spos, $esize - $spos));
			
			if (rand() < GA_MUTATION) {
				$this->mutacion($i);
			}
		}
		
	}
	function imprimir_mejor() {
		
		echo $this->contador++ . "	Respuesta: " . $this->population[0]->getstr() . "	Fitness: " . $this->population[0]->getfitness() . "<br/>";
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

$objeto = new evolutiva();
?>