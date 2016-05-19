//////////////////////////////////////////////////
// Se capturan eventos de click sobre los botones 
// de la interfaz
///////////////////////////////////////////////////
jQuery(document).ready(function($){	
    var parametros = {database_type : "parametros.database_type",
                user : "parametros.user",
                pass : "parametros.pass",
                server : "parametros.server",
                protocol : "parametros.protocol",
                port : "parametros.port",
                alias : "parametros.alias"
          };
$.ajax({
    url: "http://localhost:54607/ServiceAPI.svc/addDatabase",
    type: "GET",
    crossDomain: true,
    data: JSON.stringify(parametros),
    dataType: "json",
    contentType: "application/json; charset=utf-8",
    processData: true,
    success:function(result){
        alert(JSON.stringify(result));
    },
    error:function(xhr,status,error){
        alert(status);
    }
});
    
    
	// ANIMACIÓ INICIAL
	setTimeout(function(){
		$("header").removeClass("height-50")
		$("footer").removeClass("height-50")
		$(".ventana_principal").removeClass("opacity0")
	},2000)
	
	
	//$("#resultado").nicescroll({});	
	
	$(".problema").fileinput({
		minFileCount: 1,
		maxFileCount: 1,
		maxFileSize: "10000",

		//uploadUrl: "readTXT.php", // server upload action
		uploadAsync: false,

		browseClass: "btn btn-success",
		browseLabel: "Cargar Problema",
		dropZoneTitle: "Problema",

		showPreview: false,
		showUpload: false,
		showCaption: true,
		showRemove: false,
		
	}).on("filebatchselected", function(event, files) {
		// trigger upload method immediately after files are selected
		readTXT(files[0].name);
		archivoProblema = files[0].name;
	});
	$(".datos").fileinput({
		minFileCount: 1,
		maxFileCount: 1,
		maxFileSize: "10000",

		//uploadUrl: "readTXT.php", // server upload action
		uploadAsync: true,

		browseClass: "btn btn-success",
		browseLabel: "Cargar Datos",
		dropZoneTitle: "Datos",
		
		showPreview: false,
		showUpload: false,
		showCaption: true,
		showRemove: false,
		
	}).on("filebatchselected", function(event, files) {
		// trigger upload method immediately after files are selected
		readTXT(files[0].name);
		archivoDatos = files[0].name;

	})
	$(".salida").fileinput({
		minFileCount: 1,
		maxFileCount: 1,
		maxFileSize: "10000",

		//uploadUrl: "readTXT.php", // server upload action
		uploadAsync: true,

		browseClass: "btn btn-success",
		browseLabel: "Archivo Salida",
		dropZoneTitle: "Datos",

		showPreview: false,
		showUpload: false,
		showCaption: true,
		showRemove: false,
	}).on("filebatchselected", function(event, files) {
		// trigger upload method immediately after files are selected
		readTXT(files[0].name);
		archivoSalida = files[0].name;
	})
});

$(document).ready(function () {
    $(document).ajaxStart(function () {
        $(".bg-loading").show();
        $(".campo_grafico .bg").removeClass("gray");
    }).ajaxStop(function () {
        $(".campo_grafico .bg").addClass("gray");
		$(".bg-loading").hide();
    });
});

var archivoProblema = "";
var archivoDatos = "";
var archivoSalida = "";
function iniciar(){
	
	var generaciones = $("#generaciones").val();
	var poblacion = $("#poblacion").val();
	var mutacion = $("#mutacion").val();
	var seleccion = $("#seleccion").val();

	var last_response_len = false;
	$.ajax({
		type: "GET",
		url: "algoritmo.php",
		data:
			"generaciones=" + generaciones +
			"&poblacion=" + poblacion +
			"&mutacion=" + mutacion +
			"&seleccion=" + seleccion +
			"&archivoProblema=" + archivoProblema +
			"&archivoDatos=" + archivoDatos +
			"&archivoSalida=" + archivoSalida,
		xhrFields: {
                onprogress: function(e)
                {
                    var this_response, response = e.currentTarget.response;
                    if(last_response_len === false)
                    {
                        this_response = response;
                        last_response_len = response.length;
						$(".bg-loading").hide();

                    }
                    else
                    {
                        this_response = response.substring(last_response_len);
                        last_response_len = response.length;
                    }
                    //console.log(this_response);
					$("#resultados").append(
							this_response
					)
					$("#resultados").scrollTop($("#resultados").scrollHeight);
                }
            },
		success: function (data) {
			
		}
	});
}

function readTXT(fileName){
	$.ajax({
		type: "POST",
		url: "readTXT.php",
		data: {fileName: fileName},
		dataType: "json",
		success: function (data) {	
			$("#resultados").append("<strong>" + fileName + "</strong><br/><br/>")
			 $.each(data.txt, function (i, fb) {
				$("#resultados").append(
					"linea " + i + ": " + fb + "</br>" 
				)
			});
			$("#resultados").append("<hr/>")
			return JSON.stringify(data.txt);
		}
	});
}

function writeTXT(fileName, text){
	$.ajax({
		type: "POST",
		url: "writeTXT.php",
		data:{
			fileName: fileName, 
			text: text
		},
		success: function (data) {
		}
	});
}
