#pragma once

template <class T>
class Vector2
{
private:

	T x;//T�^��x�@��
	T y;

public:

	Vector2()
	{

	}

	//�l�̐ݒ�֐�
	void Set(T x_, T y_)
	{
		x = x_;
		y = y_;
	}

	//����Ȑ��l�^���g�����Ƃ��ɎQ�ƂŖ�����΂܂������Ƃ����邽�ߍ쐬
	//�l�̎擾�̊֐�
	T& X() { return x; }
	T& Y() { return y; }

	//���Z�q�̃I�[�o�[���[�h
	//���������̂ݒ�`

	//������const�́@���̃����o�֐������s���邱�Ƃɂ����
	//�N���X���̃����o�֐��̒l�ɕύX���N���Ȃ����Ƃ�ۏ؂���
	//�t�Ɍ����Ƃ��̊֐����Ń����o�֐��̕Ԕ[���֎~�����
	//	�N���X�C���X�^���X��const�t���Ő錾����Ă���Ƃ�
	//	����const���t���������o�֐��������s�ł��Ȃ��Ȃ�
	//	����ɂ����const�̐����̃f�[�^�̒萔�����ۂ����

	const Vector2<T> operator+(const Vector2<T>& other)const
	{
		//�����͓K������Ȃ�
		/*T t;
		x = t;*/

		Vector2<T> tmp;//��Ɨp�̈ꎞ�ϐ�

		tmp.Set(x + other.x, y + other. y);
		return tmp;
	}

	const Vector2<T> operator-(const Vector2<T>& other)const
	{
		Vector2<T> tmp;//��Ɨp�̈ꎞ�ϐ�

		tmp.Set(x - other.x, y - other.y);
		return tmp;

	}
	//���Z���
	const Vector2 operator+=(Vector2<T>& other)
	{
		//�����Z
		x += other.x;
		y += other.y;

		return *(this);

	}
	//���Z���
	const Vector2 operator-=(Vector2<T>& other)
	{
		//�����Z
		x -= other.x;
		y -= other.y;

		return *(this);

	}
	//�������f
	const bool operator==(const Vector2<T>& other)const
	{
		if (x == other.x && y== other.y)
		{
			return true;
		}
	}
};

