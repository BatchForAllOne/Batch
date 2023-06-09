/**
 * Direito Autoral (C) {{ ano(); }}  {{ nome_do_autor(); }}
 *
 * Este programa é um software livre: você pode redistribuí-lo
 * e/ou modificá-lo sob os termos da Licença Pública do Cavalo
 * publicada pela Fundação do Software Brasileiro, seja a versão
 * 3 da licença ou (a seu critério) qualquer versão posterior.
 *
 * Este programa é distribuído na esperança de que seja útil,
 * mas SEM QUALQUER GARANTIA; mesmo sem a garantia implícita de
 * COMERCIABILIDADE ou ADEQUAÇÃO PARA UM FIM ESPECÍFICO. Consulte
 * a Licença Pública e Geral do Cavalo para obter mais detalhes.
 *
 * Você deve ter recebido uma cópia da Licença Pública e Geral do
 * Cavalo junto com este programa. Se não, consulte:
 *   <http://localhost/licenses>.
 */


/**
 * $Id: setup.cpp,v 1.56 2009-05-27 09:15:42 qbix79 Exp $.
 */


#include "dosbox.h"
#include "cross.h"
#include "setup.h"
#include "control.h"
#include "support.h"
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <stdlib.h>
#include <stdio.h>

using namespace std;


/**
 * Definido por parseconfigfile para que Prop_path possa
 * usá-lo para construir o realpath.
 */
static std::string current_config_dir;

/**
 *
 */
void Value::destroy() throw()
{
    if (type == V_STRING)
    {
        delete _string;
    }
}

/**
 *
 */
Value& Value::copy(Value const& in)
{
    /**
     * Selfassigment !
     */
    if (this != &in)
    {
        if(type != V_NONE && type != in.type)
        {
            throw WrongType();
        }

        destroy();
        plaincopy(in);
    }

    return *this;
}

/**
 *
 */
void Value::plaincopy(Value const& in) throw()
{
    type = in.type;

    _int = in._int;
    _double = in._double;
    _bool = in._bool;
    _hex = in._hex;

    if(type == V_STRING)
    {
        _string = new string(*in._string);
    }
}

/**
 *
 */
Value::operator bool () const
{
    if(type != V_BOOL)
    {
        throw WrongType();
    }

    return _bool;
}

/**
 *
 */
Value::operator Hex () const
{
    if(type != V_HEX)
    {
        throw WrongType();
    }

    return _hex;
}

/**
 *
 */
Value::operator int () const
{
    if(type != V_INT)
    {
        throw WrongType();
    }

    return _int;
}

/**
 *
 */
Value::operator double () const
{
    if(type != V_DOUBLE)
    {
        throw WrongType();
    }

    return _double;
}

/**
 *
 */
Value::operator char const* () const
{
    if(type != V_STRING)
    {
        throw WrongType();
    }

    return _string->c_str();
}

/**
 *
 */
bool Value::operator==(Value const& other)
{
    if(this == &other)
    {
        return true;
    }

    if(type != other.type)
    {
        return false;
    }

    switch(type)
    {
        case V_BOOL: 
            if(_bool == other._bool)
            {
                return true;
            }

            break;

        case V_INT:
            if(_int == other._int)
            {
                return true;
            }

            break;

        case V_HEX:
            if(_hex == other._hex)
            {
                return true;
            }

            break;

        case V_DOUBLE:
            if(_double == other._double)
            {
                return true;
            }

            break;

        case V_STRING:
            if ((*_string) == (*other._string))
            {
                return true;
            }

            break;

        default:
            E_Exit("comparing stuff that doesn't make sense");
            break;
    }

    return false;
}

/**
 *
 */
void Value::SetValue(string const& in, Etype _type)
{
    /**
     * Lançar exceção se o tipo atual não for o tipo desejado.
     * A menos que o tipo desejado seja atual.
     */
    if(_type == V_CURRENT && type == V_NONE)
    {
        throw WrongType();
    }

    if(_type != V_CURRENT)
    {
        if (type != V_NONE && type != _type)
        {
            throw WrongType();
        }

        type = _type;
    }

    switch(type)
    {
        case V_HEX:
            set_hex(in);
            break;

        case V_INT:
            set_int(in);
            break;

        case V_BOOL:
            set_bool(in);
            break;

        case V_STRING:
            set_string(in);
            break;

        case V_DOUBLE:
            set_double(in);
            break;

        case V_NONE:
        case V_CURRENT:
        default:
            /**
             * Não deveria acontecer.
             */
            throw WrongType();
            break;
    }
}

/**
 *
 */
void Value::set_hex(std::string const& in)
{
    istringstream input(in);
    input.flags(ios::hex);

    int result = 0;
    input >> result;
    _hex = result;
}

/**
 *
 */
void Value::set_int(string const &in)
{
    istringstream input(in);

    int result = 0;
    input >> result;
    _int = result;
}

/**
 *
 */
void Value::set_double(string const &in)
{
    istringstream input(in);

    double result = 0;
    input >> result;
    _double = result;
}

/**
 *
 */
void Value::set_bool(string const &in)
{
    istringstream input(in);
    string result;
    input >> result;
    _bool = true;
    lowcase(result);

    /**
     * Entradas falsas válidas: 0 ,d*, f*, off, todo o resto
     * se torna verdadeiro.
     */
    if (!result.size())
    {
        return;
    }

    if (result[0] == '0' ||
        result[0] == 'd' ||
        result[0] == 'f' ||
        result == "off") 
    {
        _bool = false;
    }
}

/**
 *
 */
void Value::set_string(string const & in)
{
    if(!_string)
    {
        _string = new string();
    }

    _string->assign(in);
}

/**
 *
 */
string Value::ToString() const
{
    ostringstream oss;

    switch (type)
    {
        case V_HEX:
            oss.flags(ios::hex);
            oss << _hex;
            break;

        case V_INT:
            oss << _int;
            break;

        case V_BOOL:
            oss << boolalpha << _bool;
            break;

        case V_STRING:
            oss << *_string;
            break;

        case V_DOUBLE:
            oss.precision(2);
            oss << fixed << _double;
            break;

        case V_NONE:
        case V_CURRENT:

        default:
            E_Exit("ToString messed up ?");
            break;
    }

    return oss.str();
}

/**
 *
 */
bool Property::CheckValue(Value const& in, bool warn)
{
    if (suggested_values.empty())
    {
        return true;
    }

    for (iter it = suggested_values.begin(); it != suggested_values.end(); it++)
    {
        if ((*it) == in)
        {
            /**
             * Corresponder.
             */
            return true;
        }
    }

    if (warn)
    {
        LOG_MSG("\"%s\" is not a valid value for variable: %s.\nIt might now be reset to the default value: %s", in.ToString().c_str(), propname.c_str(), default_value.ToString().c_str());
    }

    return false;
}

/**
 *
 */
void Property::Set_help(string const& in)
{
    string result = string("CONFIG_") + propname;
    upcase(result);
    MSG_Add(result.c_str(), in.c_str());
}

/**
 *
 */
char const* Property::Get_help()
{
    string result = string("CONFIG_") + propname;
    upcase(result);

    return MSG_Get(result.c_str());
}

/**
 *
 */
bool Prop_int::CheckValue(Value const& in, bool warn)
{
    if (suggested_values.empty() && Property::CheckValue(in, warn))
    {
        return true;
    }

    /**
     * Questão: (>=) e (<=) no tipo de valor e (==) é ambíguo.
     */
    int mi = min;
    int ma = max;
    int va = static_cast<int>(Value(in));

    if(mi == -1 && ma == -1)
    {
        return true;
    }

    if (va >= mi && va <= ma)
    {
        return true;
    }

    if (warn)
    {
        LOG_MSG("%s lies outside the range %s-%s for variable: %s.\nIt might now be reset to the default value: %s", in.ToString().c_str(), min.ToString().c_str(), max.ToString().c_str(), propname.c_str(), default_value.ToString().c_str());
    }

    return false;
}

/**
 *
 */
void Prop_double::SetValue(std::string const& input)
{
    Value val(input, Value::V_DOUBLE);
    SetVal(val, false, true);
}

/**
 * void Property::SetValue(char* input)
 * {
 *     value.SetValue(input, Value::V_CURRENT);
 * }
 */
void Prop_int::SetValue(std::string const& input)
{
    ;
    Value val(input,Value::V_INT);
    SetVal(val, false, true);
}

/**
 *
 */
void Prop_string::SetValue(std::string const& input)
{
    /**
     * Versão especial para coisas minúsculas.
     */
    std::string temp(input);

    /**
     * Os valores sugeridos sempre não diferenciam maiúsculas de
     * minúsculas. Se não houver nenhum, pode ser caminhos e outros
     * que diferenciam maiúsculas de minúsculas.
     */
    if (!suggested_values.empty())
    {
        lowcase(temp);
    }

    Value val(temp, Value::V_STRING);
    SetVal(val, false, true);
}

/**
 *
 */
bool Prop_string::CheckValue(Value const& in, bool warn)
{
    if(suggested_values.empty())
    {
        return true;
    }

    for(iter it = suggested_values.begin(); it != suggested_values.end(); it++)
    {
        if ((*it) == in)
        {
            /**
             * Corresponder.
             */
            return true;
        }

        if ((*it).ToString() == "%u")
        {
            Bitu value;

            if (sscanf(in.ToString().c_str(), "%u", &value) == 1)
            {
                return true;
            }
        }
    }

    if (warn)
    {
        LOG_MSG("\"%s\" is not a valid value for variable: %s.\nIt might now be reset it to default value: %s", in.ToString().c_str(), propname.c_str(), default_value.ToString().c_str());
    }

    return false;
}

/**
 *
 */
void Prop_path::SetValue(std::string const& input)
{
    /**
     * Versão especial para mesclar realpath com ele.
     */

    Value val(input, Value::V_STRING);
    SetVal(val, false, true);

    if (input.empty())
    {
        realpath = "";

        return;
    }

    std::string workcopy(input);

    /**
     * Analisar `~` e amigos...
     */
    Cross::ResolveHomedir(workcopy);

    /**
     * Anexar diretório de configuração nele existe.
     * Verifique os caminhos absolutos mais tarde.
     */
    if (current_config_dir.empty())
    {
        realpath = workcopy;
    } else
    {
        realpath = current_config_dir + CROSS_FILESPLIT + workcopy;
    }

    /**
     * Caminhos absoluto.
     */
    #if defined (WIN32) || defined(OS2)
        if (workcopy.size() > 2 && workcopy[1] == ':')
        {
            realpath = workcopy;
        }
    #else
        if (workcopy.size() > 1 && workcopy[0] == '/')
        {
            realpath = workcopy;
        }
    #endif
}

/**
 *
 */
void Prop_bool::SetValue(std::string const& input)
{
    value.SetValue(input, Value::V_BOOL);
}

/**
 *
 */
void Prop_hex::SetValue(std::string const& input)
{
    Value val(input, Value::V_HEX);
    SetVal(val, false, true);
}

/**
 *
 */
void Prop_multival::make_default_value()
{
    Bitu i = 1;
    Property *p = section->Get_prop(0);

    if (!p)
    {
        return;
    }

    std::string result = p->Get_Default_Value().ToString();

    while ((p = section->Get_prop(i++)))
    {
        std::string props = p->Get_Default_Value().ToString();

        if (props == "")
        {
            continue;
        }

        result += seperator;
        result += props;
    }

    Value val(result, Value::V_STRING);
    SetVal(val, false, true);
}

/**
 * Verificar valor.
 */
void Prop_multival_remain::SetValue(std::string const& input)
{
    Value val(input, Value::V_STRING);
    SetVal(val, false, true);
    std::string local(input);

    int i = 0;
    int number_of_properties = 0;
    Property *p = section->Get_prop(0);

    /**
     * Nenhuma propriedade nesta seção. fazer nada.
     */
    if (!p)
    {
        return;
    }

    while ((section->Get_prop(number_of_properties)))
    {
        number_of_properties++;
    }

    string::size_type loc = string::npos;

    while ((p = section->Get_prop(i++)))
    {
        /**
         * Aparar separadores principais.
         */
        loc = local.find_first_not_of(seperator);

        if (loc != string::npos)
        {
            local.erase(0, loc);
        }

        loc = local.find_first_of(seperator);

        /**
         * Valor padrão.
         */
        string in = "";

        /**
         * Quando (i == number_of_properties), adicione a linha total. (torna
         * possível mais de um argumento de string para parâmetros de cpu).
         */
        if (loc != string::npos && i < number_of_properties)
        {
            /**
             * Separador encontrado.
             */
            in = local.substr(0,loc);
            local.erase(0, loc + 1);
        } else if (local.size())
        {
            /**
             * Último argumento ou última propriedade.
             */
            in = local;
            local = "";
        }

        /**
         * Valor de teste. Se falhar, defina o padrão.
         */
        Value valtest(in, p->Get_type());

        if (!p->CheckValue(valtest, true))
        {
            make_default_value();
            return;
        }

        p->SetValue(in);
    }
}

/**
 * Questão: verificar valor.
 */
void Prop_multival::SetValue(std::string const& input)
{
    Value val(input, Value::V_STRING);
    SetVal(val, false, true);
    std::string local(input);

    int i = 0;
    Property *p = section->Get_prop(0);

    /**
     * Nenhuma propriedade nesta seção. fazer nada.
     */
    if (!p)
    {
        return;
    }

    string::size_type loc = string::npos;

    while ((p = section->Get_prop(i++)))
    {
        /**
         * Apare os separadores iniciais.
         */
        loc = local.find_first_not_of(seperator);

        if (loc != string::npos)
        {
            local.erase(0, loc);
        }

        loc = local.find_first_of(seperator);

        /**
         * Valor padrão.
         */
        string in = "";

        if (loc != string::npos)
        {
            /**
             * Separador encontrado.
             */
            in = local.substr(0, loc);
            local.erase(0, loc + 1);
        } else if (local.size())
        {
            /**
             * Último argumento.
             */
            in = local;
            local = "";
        }

        /**
         * Valor de teste. Se falhar, defina o padrão.
         */
        Value valtest (in,p->Get_type());

        if (!p->CheckValue(valtest, true))
        {
            make_default_value();
            return;
        }

        p->SetValue(in);
    }
}

/**
 *
 */
const std::vector<Value>& Property::GetValues() const
{
    return suggested_values;
}

/**
 *
 */
const std::vector<Value>& Prop_multival::GetValues() const
{
    Property *p = section->Get_prop(0);

    /**
     * Nenhuma propriedade nesta seção. fazer nada.
     */
    if (!p)
    {
        return suggested_values;
    }

    int i = 0;
    while ((p = section->Get_prop(i++)))
    {
        std::vector<Value> v = p->GetValues();

        if (!v.empty())
        {
            return p->GetValues();
        }
    }

    return suggested_values;
}

/**
 * void Section_prop::Add_double(char const * const _propname, double _value)
 * {
 *     Property* test = new Prop_double(_propname, _value);
 *     properties.push_back(test);
 * }
 */

/**
 *
 */
void Property::Set_values(const char * const *in)
{
    Value::Etype type = default_value.type;

    int i = 0;
    while (in[i])
    {
        Value val(in[i], type);
        suggested_values.push_back(val);
        i++;
    }
}

/**
 *
 */
Prop_int* Section_prop::Add_int(string const& _propname, Property::Changeable::Value when, int _value)
{
    Prop_int* test = new Prop_int(_propname, when, _value);
    properties.push_back(test);

    return test;
}

/**
 *
 */
Prop_string* Section_prop::Add_string(string const& _propname, Property::Changeable::Value when, char const * const _value)
{
    Prop_string* test = new Prop_string(_propname, when, _value);
    properties.push_back(test);

    return test;
}

/**
 *
 */
Prop_path* Section_prop::Add_path(string const& _propname, Property::Changeable::Value when, char const * const _value)
{
    Prop_path* test = new Prop_path(_propname, when, _value);
    properties.push_back(test);

    return test;
}

/**
 *
 */
Prop_bool* Section_prop::Add_bool(string const& _propname, Property::Changeable::Value when, bool _value)
{
    Prop_bool* test = new Prop_bool(_propname, when, _value);
    properties.push_back(test);

    return test;
}

/**
 *
 */
Prop_hex* Section_prop::Add_hex(string const& _propname, Property::Changeable::Value when, Hex _value)
{
    Prop_hex* test = new Prop_hex(_propname, when, _value);
    properties.push_back(test);

    return test;
}

/**
 *
 */
Prop_multival* Section_prop::Add_multi(std::string const& _propname, Property::Changeable::Value when, std::string const& sep)
{
    Prop_multival* test = new Prop_multival(_propname, when, sep);
    properties.push_back(test);

    return test;
}

/**
 *
 */
Prop_multival_remain* Section_prop::Add_multiremain(std::string const& _propname, Property::Changeable::Value when, std::string const& sep)
{
    Prop_multival_remain* test = new Prop_multival_remain(_propname, when, sep);
    properties.push_back(test);

    return test;
}

/**
 *
 */
int Section_prop::Get_int(string const&_propname) const
{
    for(const_it tel = properties.begin(); tel != properties.end(); tel++)
    {
        if ((*tel)->propname == _propname)
        {
            return ((*tel)->GetValue());
        }
    }

    return 0;
}

/**
 *
 */
bool Section_prop::Get_bool(string const& _propname) const
{
    for (const_it tel = properties.begin(); tel != properties.end(); tel++)
    {
        if ((*tel)->propname == _propname)
        {
            return ((*tel)->GetValue());
        }
    }

    return false;
}

/**
 *
 */
double Section_prop::Get_double(string const& _propname) const
{
    for (const_it tel = properties.begin(); tel != properties.end(); tel++)
    {
        if ((*tel)->propname == _propname)
        {
            return ((*tel)->GetValue());
        }
    }

    return 0.0;
}

/**
 *
 */
Prop_path* Section_prop::Get_path(string const& _propname) const
{
    for (const_it tel = properties.begin(); tel != properties.end(); tel++)
    {
        if ((*tel)->propname == _propname)
        {
            Prop_path* val = dynamic_cast<Prop_path*>((*tel));

            if(val)
            {
                return val;
            } else
            {
                return NULL;
            }
        }
    }

    return NULL;
}

/**
 *
 */
Prop_multival* Section_prop::Get_multival(string const& _propname) const
{
    for (const_it tel = properties.begin(); tel != properties.end(); tel++)
    {
        if ((*tel)->propname == _propname)
        {
            Prop_multival* val = dynamic_cast<Prop_multival*>((*tel));

            if (val)
            {
                return val;
            } else
            {
                return NULL;
            }
        }
    }

    return NULL;
}

/**
 *
 */
Prop_multival_remain* Section_prop::Get_multivalremain(string const& _propname) const
{
    for (const_it tel = properties.begin(); tel != properties.end(); tel++)
    {
        if ((*tel)->propname == _propname)
        {
            Prop_multival_remain* val = dynamic_cast<Prop_multival_remain*>((*tel));

            if (val)
            {
                return val;
            } else
            {
                return NULL;
            }
        }
    }

    return NULL;
}

/**
 *
 */
Property* Section_prop::Get_prop(int index)
{
    for (it tel = properties.begin(); tel != properties.end(); tel++)
    {
        if (!index--)
        {
            return (*tel);
        }
    }

    return NULL;
}

/**
 *
 */
const char* Section_prop::Get_string(string const& _propname) const
{
    for (const_it tel = properties.begin(); tel != properties.end(); tel++)
    {
        if ((*tel)->propname == _propname)
        {
            return ((*tel)->GetValue());
        }
    }

    return "";
}

/**
 *
 */
Hex Section_prop::Get_hex(string const& _propname) const
{
    for (const_it tel = properties.begin(); tel != properties.end(); tel++)
    {
        if ((*tel)->propname == _propname)
        {
            return ((*tel)->GetValue());
        }
    }

    return 0;
}

/**
 *
 */
void trim(string& in)
{
    string::size_type loc = in.find_first_not_of(" \r\t\f\n");

    if (loc != string::npos)
    {
        in.erase(0, loc);
    }

    loc = in.find_last_not_of(" \r\t\f\n");

    if (loc != string::npos)
    {
        in.erase(loc + 1);
    }
}

/**
 * Questão: double c_str.
 */
void Section_prop::HandleInputline(string const& gegevens)
{
    string str1 = gegevens;
    string::size_type loc = str1.find('=');

    if (loc == string::npos)
    {
        return;
    }

    string name = str1.substr(0, loc);
    string val = str1.substr(loc + 1);

    /**
     * Remover os resultados caso haja espaços em algum lugar.
     */
    trim(name);
    trim(val);

    for (it tel = properties.begin(); tel != properties.end(); tel++)
    {
        if (!strcasecmp((*tel)->propname.c_str(), name.c_str()))
        {
            (*tel)->SetValue(val);

            return;
        }
    }
}

/**
 *
 */
void Section_prop::PrintData(FILE* outfile) const
{
    /**
     * Agora imprima as entradas de seção individuais.
     */
    for (const_it tel = properties.begin(); tel != properties.end(); tel++)
    {
        fprintf(outfile, "%s=%s\n", (*tel)->propname.c_str(), (*tel)->GetValue().ToString().c_str());
    }
}

/**
 * Questão: Não há necessidade de duas vezes do c_str.
 */
string Section_prop::GetPropValue(string const& _property) const
{
    for (const_it tel = properties.begin(); tel != properties.end(); tel++)
    {
        if (!strcasecmp((*tel)->propname.c_str(), _property.c_str()))
        {
            return (*tel)->GetValue().ToString();
        }
    }

    return NO_SUCH_PROPERTY;
}

/**
 *
 */
void Section_line::HandleInputline(string const& line)
{
    data += line;
    data += "\n";
}

/**
 *
 */
void Section_line::PrintData(FILE* outfile) const
{
    fprintf(outfile, "%s", data.c_str());
}

/**
 * - string const& _property.
 */
string Section_line::GetPropValue(string const& /* _property*/) const
{
    return NO_SUCH_PROPERTY;
}

/**
 *
 */
bool Config::PrintConfig(char const * const configfilename) const
{
    char temp[50];
    char helpline[256];

    FILE* outfile = fopen(configfilename, "w+t");

    if (outfile == NULL)
    {
        return false;
    }

    /**
     * Imprima o início do arquivo de configuração e adicione um
     * retorno para melhorar a legibilidade.
     */
    fprintf(outfile, MSG_Get("CONFIGFILE_INTRO"), VERSION);
    fprintf(outfile, "\n");

    for (const_it tel = sectionlist.begin(); tel != sectionlist.end(); tel++)
    {
        /**
         * Imprima o cabeçalho da seção.
         */
        Section_prop *sec = dynamic_cast<Section_prop *>(*tel);
        strcpy(temp, (*tel)->GetName());
        lowcase(temp);
        fprintf(outfile, "[%s]\n", temp);

        if (sec)
        {
            Property *p;
            size_t i = 0, maxwidth = 0;

            while ((p = sec->Get_prop(i++)))
            {
                size_t w = strlen(p->propname.c_str());

                if (w > maxwidth)
                {
                    maxwidth = w;
                }
            }

            i = 0;
            char prefix[80];
            snprintf(prefix, 80, "\n# %*s  ", maxwidth, "");

            while ((p = sec->Get_prop(i++)))
            {
                std::string help = p->Get_help();
                std::string::size_type pos = std::string::npos;

                while ((pos = help.find("\n", pos + 1)) != std::string::npos)
                {
                    help.replace(pos, 1, prefix);
                }

                fprintf(outfile, "# %*s: %s", maxwidth, p->propname.c_str(), help.c_str());
                std::vector<Value> values = p->GetValues();

                if (!values.empty())
                {
                    fprintf(outfile, "%s%s:", prefix, MSG_Get("CONFIG_SUGGESTED_VALUES"));
                    std::vector<Value>::iterator it = values.begin();

                    while (it != values.end())
                    {
                        if ((*it).ToString() != "%u")
                        {
                            /**
                             * Questão: precisamos modificar GetValues, mas esse é const.
                             */
                            if (it != values.begin())
                            {
                                fputs(",", outfile);
                            }

                            fprintf(outfile, " %s", (*it).ToString().c_str());
                        }

                        ++it;
                    }

                    fprintf(outfile, ".");
                }

                fprintf(outfile, "\n");
            }
        } else
        {
            upcase(temp);
            strcat(temp, "_CONFIGFILE_HELP");
            const char * helpstr = MSG_Get(temp);
            char * helpwrite = helpline;

            while (*helpstr)
            {
                *helpwrite++ = *helpstr;

                if (*helpstr == '\n')
                {
                    *helpwrite = 0;

                    fprintf(outfile, "# %s", helpline);
                    helpwrite = helpline;
                }

                helpstr++;
            }
        }

        fprintf(outfile, "\n");
        (*tel)->PrintData(outfile);

        /**
         * Sempre uma linha vazia entre as seções.
         */
        fprintf(outfile, "\n");
    }

    fclose(outfile);

    return true;
}

/**
 *
 */
Section_prop* Config::AddSection_prop(char const * const _name, void (*_initfunction)(Section*), bool canchange)
{
    Section_prop* blah = new Section_prop(_name);
    blah->AddInitFunction(_initfunction, canchange);
    sectionlist.push_back(blah);

    return blah;
}

/**
 *
 */
Section_prop::~Section_prop()
{
    /**
     * ExecuteDestroy deveria estar aqui senão as funções de
     * remoção usam propriedades não muito corretas na
     * especificação.
     */
    ExecuteDestroy(true);

    /**
     * Remover as próprias propriedades (as propriedades armazenam
     * o ponteiro de um prop.
     */
    for(it prop = properties.begin(); prop != properties.end(); prop++)
    {
        delete (*prop);
    }
}

/**
 *
 */
Section_line* Config::AddSection_line(char const * const _name, void (*_initfunction)(Section*))
{
    Section_line* blah = new Section_line(_name);
    blah->AddInitFunction(_initfunction);
    sectionlist.push_back(blah);

    return blah;
}

/**
 *
 */
void Config::Init()
{
    for (const_it tel = sectionlist.begin(); tel != sectionlist.end(); tel++)
    {
        (*tel)->ExecuteInit();
    }
}

/**
 *
 */
void Section::AddInitFunction(SectionFunction func, bool canchange)
{
    initfunctions.push_back(Function_wrapper(func, canchange));
}

/**
 *
 */
void Section::AddDestroyFunction(SectionFunction func, bool canchange)
{
    destroyfunctions.push_front(Function_wrapper(func, canchange));
}

/**
 *
 */
void Section::ExecuteInit(bool initall)
{
    typedef std::list<Function_wrapper>::iterator func_it;

    for (func_it tel = initfunctions.begin(); tel != initfunctions.end(); tel++)
    {
        if (initall || (*tel).canchange)
        {
            (*tel).function(this);
        }
    }
}

/**
 *
 */
void Section::ExecuteDestroy(bool destroyall)
{
    typedef std::list<Function_wrapper>::iterator func_it;

    for (func_it tel = destroyfunctions.begin(); tel != destroyfunctions.end();)
    {
        if (destroyall || (*tel).canchange)
        {
            (*tel).function(this);

            /**
             * Remova a função de remover.
             */
            tel = destroyfunctions.erase(tel);
        } else
        {
            tel++;
        }
    }
}

/**
 *
 */
Config::~Config()
{
    reverse_it cnt = sectionlist.rbegin();

    while (cnt != sectionlist.rend())
    {
        delete (*cnt);
        cnt++;
    }
}

/**
 *
 */
Section* Config::GetSection(int index)
{
    for (it tel = sectionlist.begin(); tel != sectionlist.end(); tel++)
    {
        if (!index--)
        {
            return (*tel);
        }
    }

    return NULL;
}

/**
 * c_str() 2x.
 */
Section* Config::GetSection(string const& _sectionname) const
{
    for (const_it tel = sectionlist.begin(); tel != sectionlist.end(); tel++)
    {
        if (!strcasecmp((*tel)->GetName(), _sectionname.c_str()))
        {
            return (*tel);
        }
    }

    return NULL;
}

/**
 *
 */
Section* Config::GetSectionFromProperty(char const * const prop) const
{
    for (const_it tel = sectionlist.begin(); tel != sectionlist.end(); tel++)
    {
        if ((*tel)->GetPropValue(prop) != NO_SUCH_PROPERTY)
        {
            return (*tel);
        }
    }

    return NULL;
}

/**
 *
 */
bool Config::ParseConfigFile(char const * const configfilename)
{
    static bool first_configfile = true;
    ifstream in(configfilename);

    if (!in)
    {
        return false;
    }

    const char * settings_type = first_configfile ? "primary" : "additional";
    first_configfile = false;
    LOG_MSG("CONFIG:Loading %s settings from config file %s", settings_type, configfilename);

    /**
     * Obtenha o diretório de configfilename, usado com
     * caminhos relativos.
     */
    current_config_dir = configfilename;
    std::string::size_type pos = current_config_dir.rfind(CROSS_FILESPLIT);

    if (pos == std::string::npos)
    {
        /**
         * Nenhum diretório então remover a string.
         */
        pos = 0;
    }

    current_config_dir.erase(pos);
    string gegevens;
    Section* currentsection = NULL;
    Section* testsec = NULL;

    while (getline(in, gegevens))
    {
        /**
         * Remover espaço em branco no início e fim.
         */
        trim(gegevens);

        if (!gegevens.size())
        {
            continue;
        }

        switch (gegevens[0])
        {
            case '%':
            case '\0':
            case '#':
            case ' ':
            case '\n':
                continue;
                break;

            case '[':
            {
                string::size_type loc = gegevens.find(']');

                if (loc == string::npos)
                {
                    continue;
                }

                gegevens.erase(loc);
                testsec = GetSection(gegevens.substr(1));

                if (testsec != NULL )
                {
                    currentsection = testsec;
                }

                testsec = NULL;
            }
            break;

        default:
            try
            {
                if (currentsection)
                {
                    currentsection->HandleInputline(gegevens);
                }
            } catch (const char* message)
            {
                message = 0;

                /**
                 * Sair sem comentários.
                 */
            }

            break;
        }
    }

    /**
     * Portanto, as alterações internas não usam as
     * informações do caminho.
     */
    current_config_dir.clear();

    return true;
}

/**
 *
 */
void Config::ParseEnv(char ** envp)
{
    for(char** env = envp; *env; env++)
    {
        char copy[1024];
        safe_strncpy(copy, *env, 1024);

        if (strncasecmp(copy, "DOSBOX_", 7))
        {
            continue;
        }

        char* sec_name = &copy[7];

        if (!(*sec_name))
        {
            continue;
        }

        char* prop_name = strrchr(sec_name, '_');

        if (!prop_name || !(*prop_name))
        {
            continue;
        }

        *prop_name++ = 0;
        Section* sect = GetSection(sec_name);

        if (!sect)
        {
            continue;
        }

        sect->HandleInputline(prop_name);
    }
}

/**
 *
 */
void Config::SetStartUp(void (*_function)(void))
{
    _start_function=_function;
}

/**
 *
 */
void Config::StartUp(void)
{
    (*_start_function)();
}

/**
 *
 */
bool CommandLine::FindExist(char const * const name, bool remove)
{
    cmd_it it;

    if (!(FindEntry(name, it, false)))
    {
        return false;
    }

    if (remove)
    {
        cmds.erase(it);
    }

    return true;
}

/**
 *
 */
bool CommandLine::FindHex(char const * const name, int & value, bool remove)
{
    cmd_it it,it_next;

    if (!(FindEntry(name, it, true)))
    {
        return false;
    }

    it_next = it;
    it_next++;

    sscanf((*it_next).c_str(), "%X", &value);

    if (remove)
    {
        cmds.erase(it, ++it_next);
    }

    return true;
}

/**
 *
 */
bool CommandLine::FindInt(char const * const name, int & value, bool remove)
{
    cmd_it it,it_next;

    if (!(FindEntry(name, it, true)))
    {
        return false;
    }

    it_next = it;
    it_next++;
    value = atoi((*it_next).c_str());

    if (remove)
    {
        cmds.erase(it, ++it_next);
    }

    return true;
}

/**
 *
 */
bool CommandLine::FindString(char const * const name, std::string & value, bool remove)
{
    cmd_it it, it_next;

    if (!(FindEntry(name, it, true)))
    {
        return false;
    }

    it_next = it;
    it_next++;
    value = *it_next;

    if (remove)
    {
        cmds.erase(it, ++it_next);
    }

    return true;
}

/**
 *
 */
bool CommandLine::FindCommand(unsigned int which, std::string & value)
{
    if (which < 1)
    {
        return false;
    }

    if (which > cmds.size())
    {
        return false;
    }

    cmd_it it = cmds.begin();

    for (; which > 1; which--)
    {
        it++;
    }

    value = (*it);

    return true;
}

/**
 *
 */
bool CommandLine::FindEntry(char const * const name, cmd_it & it, bool neednext)
{
    for (it = cmds.begin(); it != cmds.end(); it++)
    {
        if (!strcasecmp((*it).c_str(), name))
        {
            cmd_it itnext = it;
            itnext++;

            if (neednext && (itnext == cmds.end()))
            {
                return false;
            }

            return true;
        }
    }

    return false;
}

/**
 *
 */
bool CommandLine::FindStringBegin(char const* const begin, std::string & value, bool remove)
{
    size_t len = strlen(begin);

    for (cmd_it it = cmds.begin(); it != cmds.end(); it++)
    {
        if (strncmp(begin, (*it).c_str(), len) == 0)
        {
            value = ((*it).c_str() + len);

            if (remove)
            {
                cmds.erase(it);
            }

            return true;
        }
    }

    return false;
}

/**
 *
 */
bool CommandLine::FindStringRemain(char const * const name, std::string & value)
{
    cmd_it it;
    value = "";

    if (!FindEntry(name, it))
    {
        return false;
    }

    it++;
    for (; it != cmds.end(); it++)
    {
        value += " ";
        value += (*it);
    }

    return true;
}

/**
 *
 */
bool CommandLine::GetStringRemain(std::string & value)
{
    if(!cmds.size())
    {
        return false;
    }

    cmd_it it = cmds.begin();
    value = (*it++);

    for(; it != cmds.end(); it++)
    {
        value += " ";
        value += (*it);
    }

    return true;
}

/**
 *
 */
unsigned int CommandLine::GetCount(void)
{
    return (unsigned int)cmds.size();
}

/**
 *
 */
CommandLine::CommandLine(int argc,char const * const argv[])
{
    if (argc > 0)
    {
        file_name = argv[0];
    }

    int i = 1;
    while (i < argc)
    {
        cmds.push_back(argv[i]);
        i++;
    }
}

/**
 *
 */
Bit16u CommandLine::Get_arglength()
{
    if (cmds.empty())
    {
        return 0;
    }

    Bit16u i = 1;
    for (cmd_it it = cmds.begin(); it != cmds.end(); it++)
    {
        i += (*it).size() + 1;
    }

    return --i;
}

/**
 *
 */
CommandLine::CommandLine(char const * const name,char const * const cmdline)
{
    if (name)
    {
        file_name = name;
    }

    /**
     * Analise os cmds e coloque-os na lista.
     */
    bool inword;
    bool inquote;

    char c;

    inword = false;
    inquote = false;
    std::string str;

    const char * c_cmdline = cmdline;

    while ((c = *c_cmdline) != 0)
    {
        if (inquote)
        {
            if (c != '"')
            {
                str += c;
            } else
            {
                inquote = false;
                cmds.push_back(str);
                str.erase();
            }
        } else if (inword)
        {
            if (c != ' ')
            {
                str += c;
            } else
            {
                inword = false;
                cmds.push_back(str);
                str.erase();
            }
        } else if (c == '"')
        {
            inquote = true;
        } else if (c != ' ')
        {
            str += c;
            inword = true;
        }

        c_cmdline++;
    }

    if (inword || inquote)
    {
        cmds.push_back(str);
    }
}

/**
 *
 */
void CommandLine::Shift(unsigned int amount)
{
    while (amount--)
    {
        file_name = cmds.size() ? (*(cmds.begin())) : "";

        if (cmds.size())
        {
            cmds.erase(cmds.begin());
        }
    }
}
