/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package geneticos;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

/**
 *
 * @author jeffrey-debian
 */
public class GT1 extends Problema{
    
    private ArrayList<String> list;
    private int N, M, K;
    private String name;
    
    public GT1(String problem) throws IOException {
        list  = new ArrayList<String>();
        readProblema(problem);
        N = Integer.parseInt(list.get(1));
        M = Integer.parseInt(list.get(2));
        K = Integer.parseInt(list.get(3));
        name = list.get(0);
    }
    public void readProblema(String fileName) throws IOException{// Lee un problema de un archivo
        Scanner fileScanner = new Scanner(new File(fileName));
        while (fileScanner.hasNext()) {
            list.add(fileScanner.next());
        }
        fileScanner.close();
    }
    @Override
    public int geneSize(){
        return N;
    }
    @Override
    public String name() {
        return name;
    }

    @Override
    public double fitness() {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }
    
}
