import java.io.*;

/**
 * User: Gunter
 * Date: 24.07.11
 * Time: 11:26
 * <p/>
 * class performing the action...
 */
public class CAndAAction {

    String sourceFile = "";
    String destFile = "";
    long fillUpTo = 0;
    byte fillWith = 0;
    boolean doM2 = false;

    public void checkArgs(String[] args) throws IOException {

        String fillUpToArg = "";
        String fillWithArg = "";

        if (args.length < 2) {
            throw new IOException("not enough arguments!");
        }

        for (int i = 0; i < args.length; i++) {
            if (args[i].equalsIgnoreCase("-M2") || args[i].equalsIgnoreCase("/M2")) {
                doM2 = true;
                continue;
            }
            if (args[i].equalsIgnoreCase("-T") || args[i].equalsIgnoreCase("/T")) {
                i++;
                fillUpToArg = args[i];
                continue;
            }
            if (args[i].equalsIgnoreCase("-B") || args[i].equalsIgnoreCase("/B")) {
                i++;
                fillWithArg = args[i];
                continue;
            }
            if (sourceFile.length() == 0) {
                sourceFile = args[i];
                continue;
            }
            if (destFile.length() == 0) {
                destFile = args[i];
            }
        }

        if (sourceFile.length() == 0) {
            throw new IOException("no source file given!");
        }
        if (destFile.length() == 0) {
            throw new IOException("no destination file given!");
        }

        if (fillUpToArg.length() > 0) {
            fillUpTo = parseNumArgument(fillUpToArg);

            if (doM2) {
                fillUpTo *= 2;
            }

            if (fillWithArg.length() > 0) {
                fillWith = (byte) (parseNumArgument(fillWithArg) & 0xFF);
            }
        }
    }

    private long parseNumArgument(String arg) {
        int radix = 10;
        if (arg.startsWith("0x") || arg.startsWith("0X")) {
            radix = 16;
            arg = arg.substring(2);
        }
        return Long.parseLong(arg, radix);
    }

    static void displayUsage() {
        System.out.println();
        System.out.println("This small program copies a file and append bytes to a given length");
        System.out.println("Usage: java -jar copyandappend.jar <source file> <destination file> [-t <top address> [-b <append byte>]]");
        System.out.println("<source file> : file to copy");
        System.out.println("<destination file> : file to copy");
        System.out.println("-t <top address>: append bytes up to given length");
        System.out.println("-b <append byte>: byte to append");
        System.out.println("-m2: multiply top address by 2");

        System.out.println("sample: java -jar test.out test.bin -b 0x10000 -b 0x78");
    }

    void doCopyAndAppend() throws IOException {
        FileInputStream inputFile = new FileInputStream(sourceFile);
        FileOutputStream outputFile = new FileOutputStream(destFile);

        long byteCount = 0;
        int c;

        while ((c = inputFile.read()) != -1) {
            outputFile.write(c);
            byteCount++;
        }

        while (byteCount < fillUpTo) {
            outputFile.write(fillWith);
            byteCount++;
        }

        inputFile.close();
        outputFile.close();
    }
}
