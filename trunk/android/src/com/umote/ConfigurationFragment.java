package com.umote;

import java.util.Calendar;
import android.app.Fragment;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.widget.Button;

public class ConfigurationFragment extends Fragment {
	
	private UsbProxy proxy;

	private final BroadcastReceiver mUsbReceiver = new BroadcastReceiver() {

		public void onReceive(Context context, Intent intent) {
			String action = intent.getAction();
			if (UsbProxy.ACTION_USB_PERMISSION.equals(action)) {
				synchronized (this) {
					UsbDevice device = (UsbDevice) intent
							.getParcelableExtra(UsbManager.EXTRA_DEVICE);

					if (intent.getBooleanExtra(
							UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
						if (device != null) {
							save();
						}
					} else {
						System.out.println("Permission denied for device "
								+ device);
					}
				}
			}
		}
	};

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		this.proxy = new UsbProxy(getActivity(), mUsbReceiver);
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {

		View view = inflater.inflate(R.layout.configuration_fragment,
				container, false);

		// Set date button
		Button button = (Button) view.findViewById(R.id.saveDateTimeButton);
		button.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				proxy.checkUSBPermission();
			}
		});

		return view;
	}

	

	private void save() {
		proxy.send(getDateTimeFormatted());
	}

	// rtccconfig#mday#12#wday#6#mon#2#year#12#hour#12#min#10#sec#00
	private String getDateTimeFormatted() {
		Calendar calendar = Calendar.getInstance();
		StringBuilder dateTime = new StringBuilder();
		dateTime.append("rtccconfig#");
		dateTime.append(calendar.get(Calendar.DAY_OF_MONTH)).append("#");
		dateTime.append(calendar.get(Calendar.DAY_OF_WEEK)).append("#");
		dateTime.append(calendar.get(Calendar.MONTH)).append("#");
		dateTime.append(calendar.get(Calendar.YEAR)).append("#");
		dateTime.append(calendar.get(Calendar.HOUR)).append("#");
		dateTime.append(calendar.get(Calendar.MINUTE)).append("#");
		dateTime.append(calendar.get(Calendar.SECOND));
		return dateTime.toString();
	}
}
