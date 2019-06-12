package edu.ifmo.konovalov;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.extern.java.Log;

import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.Locale;
import java.util.logging.Level;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Inner class to manipulate logs
 */
@Log
@Data
@AllArgsConstructor
public class LogObject {                        //IP : CLINET :  USER :      TIMESTAMP :                METHOD : REQ : PROTO : RESPONSE : SIZE
    private static final String REGEX = "^(\\S+) (\\S+) (\\S+) \\[([\\w:/]+\\s[+\\-]\\d{4})\\] \"(\\S+) (\\S+) (\\S+)\" (\\d{3}) (\\d+)";
    private static final Pattern PATTERN = Pattern.compile(REGEX);
    private static final DateTimeFormatter LOG_FORMATTER = DateTimeFormatter.ofPattern("dd/MMM/yyyy:HH:mm:ss Z", Locale.US);
    private static final DateTimeFormatter DATE_FORMATTER = DateTimeFormatter.ofPattern("dd/MM/yyyy", Locale.US);

    private String httpMethod;
    private String httpEndpoint;
    private String httpResponseCode;
    private String httpTimestamp;

    static LogObject getObjectFromLine(String line) {
        try {
            return parseLine(line);
        } catch (RuntimeException exception) {
            log.log(Level.INFO, exception.getMessage());
            return null;
        }
    }

    private static LogObject parseLine(String line) {
        Matcher matcher = PATTERN.matcher(line);
        if (!matcher.find()) {
            throw new RuntimeException("Error while parsing line: " + line);
        }

        LocalDate logDateTime = LocalDate.parse(matcher.group(4), LOG_FORMATTER);

        return new LogObject(matcher.group(5), matcher.group(6), matcher.group(8),
                logDateTime.format(DATE_FORMATTER));

    }
}
