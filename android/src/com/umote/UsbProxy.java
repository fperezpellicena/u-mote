package com.umote;

import java.util.HashMap;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbEndpoint;
import android.hardware.usb.UsbInterface;
import android.hardware.usb.UsbManager;

class UsbProxy {
	
	
	private PendingIntent permissionIntent;
	private UsbDevice device;
	private UsbManager manager;
	private final Activity activity;
	private final int TIMEOUT = 1000;
	private final boolean forceClaim = true;
	public static final String ACTION_USB_PERMISSION = "com.android.example.USB_PERMISSION";
	
	public UsbProxy(Activity activity, BroadcastReceiver mUsbReceiver) {
		this.activity = activity;
		this.permissionIntent = PendingIntent.getBroadcast(activity, 0,
				new Intent(ACTION_USB_PERMISSION), 0);
		activity.registerReceiver(mUsbReceiver, new IntentFilter(ACTION_USB_PERMISSION));
	}
	
	public void checkUSBPermission() {
		System.out.println("checkUSBPermission");
		manager = (UsbManager) this.activity.getSystemService(
				Context.USB_SERVICE);
		System.out.println("1");
		HashMap<String, UsbDevice> deviceList = manager.getDeviceList();
		System.out.println("deviceList: " + deviceList);
		for (UsbDevice deviceValue : deviceList.values()) {
			device = deviceValue;
			System.out.println("Device: " + device);
			manager.requestPermission(device, permissionIntent);
		}
	}
	
	public void send(String data) {
		byte[] bytes = data.getBytes();
		UsbInterface intf = device.getInterface(0);
		UsbEndpoint endpoint = intf.getEndpoint(0);
		UsbDeviceConnection connection = manager.openDevice(device);
		connection.claimInterface(intf, forceClaim);
		connection.bulkTransfer(endpoint, bytes, bytes.length, TIMEOUT);
	}
	
}
