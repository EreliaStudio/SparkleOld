#pragma once

namespace spk
{
	template <typename TType>
	class Memento
	{
	public:
		class Snapshot
		{
		private:
			TType _objectToCapture;

		public:
			constexpr void save(const TType* p_objectToCapture)
			{
				_objectToCapture = *p_objectToCapture;
			}
			constexpr void load(TType* p_objectToRestore) const
			{
				*p_objectToRestore = _objectToCapture;
			}
		};
	private:
		TType& _objectTracked;

	public:
		Memento() = delete;

		Memento (TType& p_objectToCapture):
			_objectTracked(p_objectToCapture)
		{
		}

		Memento(const Memento& p_other) = delete;
		Memento& operator=(const Memento& p_other) = delete;

		constexpr Snapshot* takeSnapshot() const
		{
			Snapshot* result = new Snapshot();

			result->save(&_objectTracked);

			return result;
		}

		constexpr void restoreSnapshot(const Snapshot* p_snapshot)
		{
			p_snapshot->load(&_objectTracked);
		}
	};
}