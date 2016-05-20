package geneticos;

import java.io.IOException;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author jeffrey-debian
 */
public abstract class Problema {
        
    public abstract void readProblema(String fileName) throws IOException;
    public abstract int geneSize();
    public abstract String name();
    public abstract double fitness();
}
