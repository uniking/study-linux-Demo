package test_json;

import java.security.Timestamp;

public class Product {
	
	private String name;
	private int id;
	private Timestamp date;

	public Product() {
		// TODO Auto-generated constructor stub
	}

	public Product(String name, int id, Timestamp date) {
		super();
		this.name = name;
		this.id = id;
		this.date = date;
	}



	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public Timestamp getDate() {
		return date;
	}

	public void setDate(Timestamp date) {
		this.date = date;
	}
	
}
