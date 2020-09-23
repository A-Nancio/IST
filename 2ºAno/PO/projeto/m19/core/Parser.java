package m19.core;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.FileReader;
import java.io.Reader;

import m19.core.Library;
import m19.core.exception.BadEntrySpecificationException;
import m19.core.user.User;
import m19.core.work.Book;
import m19.core.work.Category;
import m19.core.work.DVD;

public class Parser {

    private Library _library;

    Parser(Library lib) {
        _library = lib;
    }

    void parseFile(String filename) throws IOException, BadEntrySpecificationException {
        try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            String line;

            while ((line = reader.readLine()) != null)
                parseLine(line);
        }
    }

    private void parseLine(String line) throws BadEntrySpecificationException {
        String[] components = line.split(":");

        switch(components[0]) {
            case "DVD":
                parseDVD(components, line);
                break;

            case "BOOK":
                parseBook(components, line);
                break;
            case "USER":
                parseUser(components, line);
                break;

            default:
                throw new BadEntrySpecificationException("Invalid type " + components[0] +
                        " in line " + line);
        }
    }

    private void parseDVD(String[] components, String line) throws BadEntrySpecificationException {
        if (components.length != 7)
            throw new BadEntrySpecificationException("Wrong number of fields (6) in " + line);
        _library.addDVD(components[1], components[2], Integer.parseInt(components[3]),
                Category.valueOf(components[4]), Integer.parseInt(components[5]),
                Integer.parseInt(components[6]));
    }

    private void parseBook(String[] components, String line) throws BadEntrySpecificationException {
        if (components.length != 7)
            throw new BadEntrySpecificationException("Wrong number of fields (6) in " + line);
        _library.addBook(components[1], components[2], Integer.parseInt(components[3]),
                Category.valueOf(components[4]), Integer.parseInt(components[5]),
                Integer.parseInt(components[6]));
    }

    private void parseUser(String[] components, String line) throws BadEntrySpecificationException {
        if (components.length != 3)
            throw new BadEntrySpecificationException("Wrong number of fields (2) in " + line);
        _library.newUser(components[1], components[2]);
    }

}