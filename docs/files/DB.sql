-- Created by Redgate Data Modeler (https://datamodeler.redgate-platform.com)
-- Last modification date: 2026-04-10 07:40:49.535

-- tables
-- Table: Brands
CREATE TABLE Brands (
    id integer NOT NULL CONSTRAINT Brands_pk PRIMARY KEY AUTOINCREMENT,
    brand text NOT NULL
);

-- Table: Cars
CREATE TABLE Cars (
    state_number text NOT NULL CONSTRAINT Cars_pk PRIMARY KEY,
    brand text NOT NULL,
    release_year integer NOT NULL,
    tech_passport text NOT NULL,
    owner_id integer NOT NULL,
    CONSTRAINT Cars_Users FOREIGN KEY (owner_id)
    REFERENCES Users (id)
);

-- Table: Masters
CREATE TABLE Masters (
    id integer NOT NULL CONSTRAINT id PRIMARY KEY AUTOINCREMENT,
    name text NOT NULL,
    workshop_id integer NOT NULL,
    CONSTRAINT Masters_Workshops FOREIGN KEY (workshop_id)
    REFERENCES Workshops (id)
);

-- Table: Orders
CREATE TABLE Orders (
    id integer NOT NULL CONSTRAINT Orders_pk PRIMARY KEY AUTOINCREMENT,
    state_number text NOT NULL,
    workshop_id integer NOT NULL,
    master_id integer NOT NULL,
    start_date date NOT NULL,
    end_date date NOT NULL,
    repair_id integer NOT NULL,
    final_cost decimal(10,2) NOT NULL,
    CONSTRAINT Masters_Orders FOREIGN KEY (master_id)
    REFERENCES Masters (id),
    CONSTRAINT Orders_Workshops FOREIGN KEY (workshop_id)
    REFERENCES Workshops (id),
    CONSTRAINT Orders_Cars FOREIGN KEY (state_number)
    REFERENCES Cars (state_number),
    CONSTRAINT Orders_Repairs FOREIGN KEY (repair_id)
    REFERENCES Repairs (id)
);

-- Table: Repairs
CREATE TABLE Repairs (
    id integer NOT NULL CONSTRAINT Repairs_pk PRIMARY KEY AUTOINCREMENT,
    name text NOT NULL,
    cost decimal(10,2) NOT NULL
);

-- Table: Users
CREATE TABLE Users (
    id integer NOT NULL CONSTRAINT Users_pk PRIMARY KEY AUTOINCREMENT,
    login text NOT NULL,
    password text NOT NULL,
    name text NOT NULL,
    email text NOT NULL,
    address text NOT NULL
);

-- Table: Workshop_brands
CREATE TABLE Workshop_brands (
    workshop_id integer NOT NULL,
    brand_id integer NOT NULL,
    CONSTRAINT Workshop_brands_Brands FOREIGN KEY (brand_id)
    REFERENCES Brands (id),
    CONSTRAINT Workshop_brands_Workshops FOREIGN KEY (workshop_id)
    REFERENCES Workshops (id)
);

-- Table: Workshops
CREATE TABLE Workshops (
    id integer NOT NULL CONSTRAINT id PRIMARY KEY AUTOINCREMENT,
    address text NOT NULL
);

-- End of file.

