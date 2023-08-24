JAVA极速版

- [ ] 基础语法（边看边学）
- [ ] 字符串用法
- [ ] 类

  - [ ] 访问修饰符
  - [ ] 构造，定义，使用
  - [ ] Static
  - [ ] 继承
  - [ ] 重载
- [ ] 接口
- [ ] ArrayList,HashMap,TreeSet (Set,List，Map)
- [ ] 异常处理
- [ ] GUI事件处理
- [ ] 文件读写
- [ ] 随机访问文件类？
- [ ] 多线程（start,run……）
- [ ] 语法基础
- [ ] JDBC
- [ ] TCP网络通讯



要特别注意的是，如果我们自定义了一个构造方法，那么，编译器就*不再*自动创建默认构造方法：

如果既要能使用带参数的构造方法，又想保留不带参数的构造方法，那么只能把两个构造方法都定义出来：

不能把父类变为子类，因为子类功能比父类多，多的功能无法凭空变出来。



```java
try (Connection conn = DriverManager.getConnection(JDBC_URL, JDBC_USER, JDBC_PASSWORD)) {
    try (PreparedStatement ps = conn.prepareStatement("SELECT id, grade, name, gender FROM students WHERE gender=? AND grade=?")) {
        ps.setObject(1, "M"); // 注意：索引从1开始
        ps.setObject(2, 3);
        try (ResultSet rs = ps.executeQuery()) {
            while (rs.next()) {
                long id = rs.getLong("id");
                long grade = rs.getLong("grade");
                String name = rs.getString("name");
                String gender = rs.getString("gender");
            }
        }
    }
}

try (Connection conn = DriverManager.getConnection(JDBC_URL, JDBC_USER, JDBC_PASSWORD)) {
    try (PreparedStatement ps = conn.prepareStatement(
            "INSERT INTO students (id, grade, name, gender) VALUES (?,?,?,?)")) {
        ps.setObject(1, 999); // 注意：索引从1开始
        ps.setObject(2, 1); // grade
        ps.setObject(3, "Bob"); // name
        ps.setObject(4, "M"); // gender
        int n = ps.executeUpdate(); // 1
    }
}
try (Connection conn = DriverManager.getConnection(JDBC_URL, JDBC_USER, JDBC_PASSWORD)) {
    try (PreparedStatement ps = conn.prepareStatement("DELETE FROM students WHERE id=?")) {
        ps.setObject(1, 999); // 注意：索引从1开始
        int n = ps.executeUpdate(); // 删除的行数
    }
}
```

