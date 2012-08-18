package com.umote;

import java.text.SimpleDateFormat;

import org.afree.chart.AFreeChart;
import org.afree.chart.ChartFactory;
import org.afree.chart.axis.DateAxis;
import org.afree.chart.plot.XYPlot;
import org.afree.chart.renderer.xy.XYItemRenderer;
import org.afree.chart.renderer.xy.XYLineAndShapeRenderer;
import org.afree.data.time.Month;
import org.afree.data.time.TimeSeries;
import org.afree.data.time.TimeSeriesCollection;
import org.afree.graphics.SolidColor;
import org.afree.ui.RectangleInsets;

import android.app.Fragment;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

public class MonitorFragment extends Fragment {

    private UsbProxy proxy;
    private View view;
    private AFreeChart chart;
    private TimeSeries serie;
    private TimeSeriesCollection dataset;
    private int month = 1;
    private int year = 2012;
    private Thread thread = new Thread(new Runnable() {
	    
	    @Override
	    public void run() {
		while(true) {
		    try{
			Thread.sleep(3000);
			serie.add(new Month(month++, year), month * 50);
			if(month == 12) {
			    month = 1;
			    year = year + 1;
			}
		    }catch(InterruptedException ex){
			System.out.println(ex.toString());
		    }
		    
		}
	    }
	});

    private final BroadcastReceiver mUsbReceiver = new BroadcastReceiver() {

	@Override
	public void onReceive(Context context, Intent intent) {
	    final String action = intent.getAction();
	    if (UsbProxy.ACTION_USB_PERMISSION.equals(action)) {
		synchronized (this) {
		    final UsbDevice device = (UsbDevice) intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);

		    if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
			if (device != null) {
			    System.out.println("Permiso concedido");
			}
		    } else {
			System.out.println("Permission denied for device " + device);
		    }
		}
	    }
	}
    };

    @Override
    public void onCreate(Bundle savedInstanceState) {
	super.onCreate(savedInstanceState);
	this.thread.start();
    }
    
    

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
	this.proxy = new UsbProxy(this.getActivity(), this.mUsbReceiver);
	this.view = inflater.inflate(R.layout.monitor_fragment, container, false);
	this.serie = new TimeSeries("My serie");
	this.dataset = new TimeSeriesCollection();
	this.dataset.addSeries(this.serie);

	this.chart = ChartFactory.createTimeSeriesChart("My chart", // title
		"Date", // x-axis label
		"Price Per Unit", // y-axis label
		this.dataset, // data
		true, // create legend?
		true, // generate tooltips?
		false // generate URLs?
		);

	this.chart.setBackgroundPaintType(new SolidColor(Color.WHITE));

	final XYPlot plot = (XYPlot) this.chart.getPlot();
	plot.setBackgroundPaintType(new SolidColor(Color.LTGRAY));
	plot.setDomainGridlinePaintType(new SolidColor(Color.WHITE));
	plot.setRangeGridlinePaintType(new SolidColor(Color.WHITE));
	plot.setAxisOffset(new RectangleInsets(5.0, 5.0, 5.0, 5.0));
	plot.setDomainCrosshairVisible(true);
	plot.setRangeCrosshairVisible(true);

	final XYItemRenderer r = plot.getRenderer();
	if (r instanceof XYLineAndShapeRenderer) {
	    final XYLineAndShapeRenderer renderer = (XYLineAndShapeRenderer) r;
	    renderer.setBaseShapesVisible(true);
	    renderer.setBaseShapesFilled(true);
	    renderer.setDrawSeriesLineAsPath(true);
	}

	final DateAxis axis = (DateAxis) plot.getDomainAxis();
	axis.setDateFormatOverride(new SimpleDateFormat("MMM-yyyy"));

	this.view = new DemoView(this.getActivity());
	((DemoView) this.view).setChart(this.chart);
	
	return this.view;
    }

}
