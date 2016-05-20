/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package geneticos;

import java.io.IOException;

/**
 *
 * @author jeffrey-debian
 */
public class Geneticos {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws IOException {
        GT1 problema = new GT1("problemas/gt1.txt");
        System.out.println(problema.geneSize());
        System.out.println(problema.name());
    }
    
}
