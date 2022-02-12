//
// Created by lhx on 1/24/22.
//

#ifndef MYBLOCKCHAIN_ORM_FILED_H
#define MYBLOCKCHAIN_ORM_FILED_H
#include <string>
namespace AriaORM {
    //字段类型
    enum class ORMFiledType {
        AutoField = 0,
        BinaryField = 1,
        BooleanField = 2,
        IntegerFiled = 3,
        PositiveIntegerFiled = 4,
        CharFiled = 5,
        TextFiled = 6,
        DateTimeField = 7,
        FloatFiled = 8,
        DecimalFiled = 9,
        ForeignKey = 10,
        OneToOneField = 11, // ForeignKey == true, on_delete method
        ManyToManyField = 12 // ForeignKey == true
    };

    //字段的属性 以及字段属性的设置、访问接口
    class ORMFieldBase {
    public:
        ORMFiledType getFiledType () { return filedType; }
        std::string getColumnName() {return columnName; }

        void setPrimary(bool _primary) { primary = _primary; }
        bool isPrimary() { return primary; }

        void setUnique(bool _unique) { unique = _unique; }
        bool isUnique() { return unique; }

        void setNull(bool _nil) { nil = _nil; }
        bool isNull() { return nil; }

        void setIndex(bool _index) { index = _index; }
        bool isIndex() { return index; }

    protected:
        ORMFieldBase(ORMFiledType _filedType, std::string _columnName) :
        filedType(_filedType), columnName(_columnName) {}

    protected:
        const ORMFiledType filedType;
        const std::string columnName;
        bool primary = false;
        bool unique = false;
        bool nil = false;
        bool index = false;
    };
}
#endif //MYBLOCKCHAIN_ORM_FILED_H
