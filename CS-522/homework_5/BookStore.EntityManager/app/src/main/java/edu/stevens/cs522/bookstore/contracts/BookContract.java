package edu.stevens.cs522.bookstore.contracts;

import android.content.ContentValues;
import android.database.Cursor;
import android.net.Uri;
import android.provider.BaseColumns;

import java.util.regex.Pattern;

import edu.stevens.cs522.bookstore.entities.Author;

/**
 * Created by dduggan.
 */

public class BookContract implements BaseColumns {

    public static final String AUTHORITY = "edu.stevens.cs522.bookstore";

    public static final Uri CONTENT_URI(String authority, String path) {
        return new Uri.Builder().scheme("content")
                .authority(authority)
                .path(path)
                .build();
    }

    public static final Uri CONTENT_URI = CONTENT_URI(AUTHORITY, "Book");

    public static Uri withExtendedPath(Uri uri,
                                       String... path) {
        Uri.Builder builder = uri.buildUpon();
        for (String p : path)
            builder.appendPath(p);
        return builder.build();
    }

    public static final Uri CONTENT_URI(long id) {
        return CONTENT_URI(Long.toString(id));
    }
    public static final Uri CONTENT_URI(String id) {
        return withExtendedPath(CONTENT_URI, id);
    }
    public static final long getId(Uri uri) {
        return Long.parseLong(uri.getLastPathSegment());
    }
    public static final String CONTENT_PATH(Uri uri) {
        return uri.getPath().substring(1);
    }
    public static final String CONTENT_PATH = CONTENT_PATH(CONTENT_URI);
    public static final String CONTENT_PATH_ITEM = CONTENT_PATH(CONTENT_URI("#"));

    public static final String ID = _ID;
    public static final String TITLE = "title";
    public static final String AUTHORS = "authors";
    public static final String ISBN = "isbn";
    public static final String PRICE = "price";

    public static String contentType(String content) {
        return "vnd.android.cursor/vnd." + AUTHORITY + "." + content + "s";
    }

    public static String contentItemType(String content) {
        return "vnd.android.cursor.item/vnd." + AUTHORITY + "." + content;
    }

    /*
     * id column
     */
    private static int idColumn = -1;
    public static long getId(Cursor cursor) {
        if (idColumn < 0) {
            idColumn = cursor.getColumnIndexOrThrow(ID);
        }
        return cursor.getLong(idColumn);
    }

    /*
     * title column
     */
    private static int titleColumn = -1;
    public static String getTitle(Cursor cursor) {
        if (titleColumn < 0) {
            titleColumn = cursor.getColumnIndexOrThrow(TITLE);
        }
        return cursor.getString(titleColumn);
    }

    public static void putTitle(ContentValues values, String title) {
        values.put(TITLE, title);
    }

    /*
     * Author "column"
     */
    public static final char SEPARATOR_CHAR = '|';
    private static final Pattern SEPARATOR =
            Pattern.compile(Character.toString(SEPARATOR_CHAR), Pattern.LITERAL);

    public static String[] readStringArray(String in) {
        return SEPARATOR.split(in);
    }

    private static int authorColumn = -1;
    public static String[] getAuthors(Cursor cursor) {
        if (authorColumn < 0) {
            authorColumn = cursor.getColumnIndexOrThrow(AUTHORS);
        }
        return readStringArray(cursor.getString(authorColumn));
    }
    public static void putAuthors(ContentValues values, String authors) {
        values.put(AUTHORS, authors);
    }

    /*
     * ISBN column
     */
    private static int isbnColumn = -1;
    public static String getIsbn(Cursor cursor) {
        if (isbnColumn < 0) {
            isbnColumn = cursor.getColumnIndexOrThrow(ISBN);
        }
        return cursor.getString(isbnColumn);
    }
    public static void putIsbn(ContentValues values, String isbn) {
        values.put(ISBN, isbn);
    }

    /*
     * Price column
     */
    private static int priceColumn = -1;
    public static String getPrice(Cursor cursor) {
        if (priceColumn < 0) {
            priceColumn = cursor.getColumnIndexOrThrow(PRICE);
        }
        return cursor.getString(priceColumn);
    }
    public static void putPrice(ContentValues values, String price) {
        values.put(PRICE, price);
    }
}
