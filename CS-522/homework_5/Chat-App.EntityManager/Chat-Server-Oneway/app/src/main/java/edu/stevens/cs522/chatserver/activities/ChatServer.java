/*********************************************************************

    Chat server: accept chat messages from clients.
    
    Sender name and GPS coordinates are encoded
    in the messages, and stripped off upon receipt.

    Copyright (c) 2017 Stevens Institute of Technology

**********************************************************************/
package edu.stevens.cs522.chatserver.activities;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.Uri;
import android.os.Bundle;
import android.os.StrictMode;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.Date;
import java.util.List;

import edu.stevens.cs522.chatserver.R;
import edu.stevens.cs522.chatserver.async.IContinue;
import edu.stevens.cs522.chatserver.async.QueryBuilder;
import edu.stevens.cs522.chatserver.contracts.MessageContract;
import edu.stevens.cs522.chatserver.contracts.PeerContract;
import edu.stevens.cs522.chatserver.entities.Message;
import edu.stevens.cs522.chatserver.entities.Peer;
import edu.stevens.cs522.chatserver.managers.MessageManager;
import edu.stevens.cs522.chatserver.managers.PeerManager;
import edu.stevens.cs522.chatserver.managers.TypedCursor;

public class ChatServer extends Activity implements OnClickListener, QueryBuilder.IQueryListener<Message> {

	final static public String TAG = ChatServer.class.getCanonicalName();
		
	/*
	 * Socket used both for sending and receiving
	 */
	private DatagramSocket serverSocket; 

	/*
	 * True as long as we don't get socket errors
	 */
	private boolean socketOK = true; 

    /*
     * UI for displayed received messages
     */
	private ListView messageList;
    private SimpleCursorAdapter messagesAdapter;
    private MessageManager messageManager;
    private PeerManager peerManager;
    private Button next;

    /*
     * Use to configure the app (user name and port)
     */
    private SharedPreferences settings;
	
	/*
	 * Called when the activity is first created. 
	 */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

        /**
         * Let's be clear, this is a HACK to allow you to do network communication on the messages thread.
         * This WILL cause an ANR, and is only provided to simplify the pedagogy.  We will see how to do
         * this right in a future assignment (using a Service managing background threads).
         */
        StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy);

        /**
         * Initialize settings to default values.
         */
		if (savedInstanceState == null) {
			PreferenceManager.setDefaultValues(this, R.xml.settings, false);
		}

        settings = PreferenceManager.getDefaultSharedPreferences(this);

        int port = Integer.valueOf(settings.getString(SettingsActivity.APP_PORT_KEY, getResources().getString(R.string.default_app_port)));

		try {
			serverSocket = new DatagramSocket(port);
		} catch (Exception e) {
			Log.e(TAG, "Cannot open socket", e);
			return;
		}

        setContentView(R.layout.messages);

        this.messageList = (ListView) findViewById(R.id.message_list);
        String[] from = { MessageContract.MESSAGE_TEXT };
        int[] to = { R.id.message_text };
        this.messagesAdapter = new SimpleCursorAdapter(this, R.layout.message, null, from, to, 0);
        this.messageList.setAdapter(this.messagesAdapter);

        this.next = (Button) findViewById(R.id.next);
        this.next.setOnClickListener(this);

        this.messageManager = new MessageManager(this);
        this.peerManager = new PeerManager(this);
        this.messageManager.getAllMessagesAsync(this);
	}

    public void onDestroy() {
        super.onDestroy();
        closeSocket();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        super.onCreateOptionsMenu(menu);
        getMenuInflater().inflate(R.menu.chatserver_menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        super.onOptionsItemSelected(item);
        switch(item.getItemId()) {

            case R.id.peers:
                Intent i = new Intent(this, ViewPeersActivity.class);
                startActivity(i);
                break;

            case R.id.settings:
                Intent intent = new Intent(this, SettingsActivity.class);
                startActivity(intent);
                break;

            default:
        }
        return false;
    }



    public void onClick(View v) {
		byte[] receiveData = new byte[1024];
		DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);

		try {
			
			serverSocket.receive(receivePacket);
			Log.i(TAG, "Received a packet");

			InetAddress sourceIPAddress = receivePacket.getAddress();
			Log.i(TAG, "Source IP Address: " + sourceIPAddress);
			
			String msgContents[] = new String(receivePacket.getData(), 0, receivePacket.getLength()).split(":");

            final Message message = new Message();
            message.sender = msgContents[0];
            message.timestamp = new Date(Long.parseLong(msgContents[1]));
            message.messageText = msgContents[2];

			Log.i(TAG, "Received from " + message.sender + ": " + message.messageText);

            Peer sender = new Peer();
            sender.name = message.sender;
            sender.timestamp = message.timestamp;
            sender.address = receivePacket.getAddress();
            sender.port = receivePacket.getPort();

            peerManager.persistAsync(sender, new IContinue<Uri>() {
                @Override
                public void kontinue(Uri id) {
                    message.senderId = PeerContract.getId(id);
                    messageManager.persistAsync(message);
                }
            });
        } catch (Exception e) {
			Log.e(TAG, "Problems receiving packet: " + e.getMessage());
			socketOK = false;
		} 

	}

	/*
	 * Close the socket before exiting application
	 */
	public void closeSocket() {
		serverSocket.close();
	}

	/*
	 * If the socket is OK, then it's running
	 */
	boolean socketIsOK() {
		return socketOK;
	}

    @Override
    public void handleResults(TypedCursor<Message> results) {
        this.messagesAdapter.swapCursor(results.getCursor());
        this.messagesAdapter.notifyDataSetChanged();
    }

    @Override
    public void closeResults() {
        this.messagesAdapter.swapCursor(null);
        this.messagesAdapter.notifyDataSetChanged();
    }
}