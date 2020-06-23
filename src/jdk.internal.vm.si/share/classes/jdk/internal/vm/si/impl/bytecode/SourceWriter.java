package jdk.internal.vm.si.impl.bytecode;

import java.time.Instant;
import java.util.Objects;

public class SourceWriter {
	private final StringBuilder builder;
	
	public SourceWriter(StringBuilder builder) {
		this.builder = builder;
	}
	
	public SourceWriter() {
		this.builder = new StringBuilder();
	}
	
	public SourceWriter appendHeader(String filename) {
		return append("/* GENERATED " + filename + " FILE -- DO NOT EDIT (" + Instant.now() + ") */\n");
	}
	
	public SourceWriter append(String value) {
		this.builder.append(value);
		return this;
	}
	
	public SourceWriter append(char value) {
		this.builder.append(value);
		return this;
	}
	
	public SourceWriter append(int value) {
		this.builder.append(value);
		return this;
	}
	
	public SourceWriter append(int value, int padToLength) {
		return append(Integer.toString(value), padToLength);
	}
	
	public SourceWriter append(String value, int padToLength) {
		Objects.requireNonNull(value);
		this.builder.append(value);
		if (value.length() < padToLength) {
			for (int i = value.length(); i < padToLength; i++) {
				this.builder.append(' ');
			}
		}
		return this;
	}
	
	public StringBuilder unwrap() {
		return builder;
	}
}
