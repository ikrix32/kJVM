class Ball extends Exception {
}

class My {

	private static Ball ball = new Ball();

	static void playBall() {
		int i = 0;
		while (true) {
			try {
				if (i % 4 == 3)
					throw ball;
				++i;
			} catch (Ball b) {
				i = 0;
			}
			System.out.println(i);
		}
	}

	public static void main(String[] args) {
		new My().playBall();
	}

}
