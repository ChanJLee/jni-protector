package com.chan.lib;

/**
 * Created by chan on 2018/4/16.
 */

public class Yasc {
	static {
		System.loadLibrary("yasc");
	}

	public static native int load();
}
